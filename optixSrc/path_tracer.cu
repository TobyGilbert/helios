
/*
 * Copyright (c) 2008 - 2009 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property and proprietary
 * rights in and to this software, related documentation and any modifications thereto.
 * Any use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from NVIDIA Corporation is strictly
 * prohibited.
 *
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED *AS IS*
 * AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS BE LIABLE FOR ANY
 * SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF
 * BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
 * INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES
 */

#include <optix.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
#include "helpers.h"
#include "Core/path_tracer.h"
#include "Core/random.h"
#include "BRDFUtils.h"


// Scene wide
rtDeclareVariable(float,         scene_epsilon, , );
rtDeclareVariable(rtObject,      top_object, , );

// For camera
rtDeclareVariable(float3,        eye, , );
rtDeclareVariable(float3,        U, , );
rtDeclareVariable(float3,        V, , );
rtDeclareVariable(float3,        W, , );
rtDeclareVariable(float3,        bad_color, , );
rtDeclareVariable(unsigned int,  frame_number, , );
rtDeclareVariable(unsigned int,  sqrt_num_samples, , );
rtBuffer<float4, 2>              output_buffer;
rtBuffer<ParallelogramLight>     lights;

rtDeclareVariable(unsigned int,  pathtrace_ray_type, , );
rtDeclareVariable(unsigned int,  pathtrace_shadow_ray_type, , );
rtDeclareVariable(unsigned int,  rr_begin_depth, , );

rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal,   attribute shading_normal, );
rtDeclareVariable(float3, texcoord, attribute texcoord, );

rtDeclareVariable(PerRayData_pathtrace, current_prd, rtPayload, );

// Environment map
rtTextureSampler<float4, 2> envmap;
// Normal map
rtTextureSampler<float4, 2> normalMap;

rtDeclareVariable(optix::Ray, ray,          rtCurrentRay, );
rtDeclareVariable(float,      t_hit,        rtIntersectionDistance, );
rtDeclareVariable(uint2,      launch_index, rtLaunchIndex, );


static __device__ inline float3 powf(float3 a, float exp){
    return make_float3(powf(a.x, exp), powf(a.y, exp), powf(a.z, exp));
}

// For miss program
rtDeclareVariable(float3,       bg_color, , );

//-----------------------------------------------------------------------------
//
//  Camera program -- main ray tracing loop
//
//-----------------------------------------------------------------------------
RT_PROGRAM void pathtrace_camera(){
    size_t2 screen = output_buffer.size();

    float2 inv_screen = 1.0f/make_float2(screen) * 2.f;
    float2 pixel = (make_float2(launch_index)) * inv_screen - 1.f;

    float2 jitter_scale = inv_screen / sqrt_num_samples;
    unsigned int samples_per_pixel = sqrt_num_samples*sqrt_num_samples;
    float3 result = make_float3(0.0f);

    unsigned int seed = tea<16>(screen.x*launch_index.y+launch_index.x, frame_number);
    do {
        unsigned int x = samples_per_pixel%sqrt_num_samples;
        unsigned int y = samples_per_pixel/sqrt_num_samples;
        float2 jitter = make_float2(x-rnd(seed), y-rnd(seed));
        float2 d = pixel + jitter*jitter_scale;
        float3 ray_origin = eye;
        float3 ray_direction = normalize(d.x*U + d.y*V + W);


        PerRayData_pathtrace prd;
        prd.result = make_float3(0.f);
        prd.attenuation = make_float3(1.f);
        prd.countEmitted = true;
        prd.done = false;
        prd.inside = false;
        prd.seed = seed;
        prd.depth = 0;
        prd.type = cameraRay;

    for(;;) {
        Ray ray = make_Ray(ray_origin, ray_direction, pathtrace_ray_type, scene_epsilon, RT_DEFAULT_MAX);
        rtTrace(top_object, ray, prd);
        if(prd.done) {
            prd.result += prd.radiance * prd.attenuation;
            break;
        }

        // RR
        if(prd.depth >= rr_begin_depth){
            float pcont = fmaxf(prd.attenuation);
            if(rnd(prd.seed) >= pcont)
                break;
                prd.attenuation /= pcont;
            }
            prd.depth++;
            prd.result += prd.radiance * prd.attenuation;
            ray_origin = prd.origin;
            ray_direction = prd.direction;
        } // eye ray

        result += prd.result;
        seed = prd.seed;
    } while (--samples_per_pixel);

    float3 pixel_color = result/(sqrt_num_samples*sqrt_num_samples);

    if (frame_number > 1){
        float a = 1.0f / (float)frame_number;
        float b = ((float)frame_number - 1.0f) * a;
        float3 old_color = make_float3(output_buffer[launch_index]);
        output_buffer[launch_index] = make_float4(a * pixel_color + b * old_color, 0.0f);
    }
    else{
        output_buffer[launch_index] = make_float4(pixel_color, 0.0f);
    }
}
//-----------------------------------------------------------------------------
RT_PROGRAM void defaultMaterial(){
    if (current_prd.depth > 50){
        current_prd.done = true;
        return;

    }    
    float3 world_shading_normal   = normalize( rtTransformNormal( RT_OBJECT_TO_WORLD, shading_normal ) );
    float3 world_geometric_normal = normalize( rtTransformNormal( RT_OBJECT_TO_WORLD, geometric_normal ) );

    float3 ffnormal = faceforward( world_shading_normal, -ray.direction, world_geometric_normal );

    float3 hitpoint = ray.origin + t_hit * ray.direction;
    current_prd.origin = hitpoint;

    float z1=rnd(current_prd.seed);
    float z2=rnd(current_prd.seed);
    float3 p;
    cosine_sample_hemisphere(z1, z2, p);
    float3 v1, v2;
    createONB(ffnormal, v1, v2);
    current_prd.direction = v1 * p.x + v2 * p.y + ffnormal * p.z;

//    current_prd.countEmitted = false;

    // Compute direct light...
    // Or shoot one...
    unsigned int num_lights = lights.size();
    float3 result = make_float3(0.0f);

    for(int i = 0; i < num_lights; ++i) {
        ParallelogramLight light = lights[i];
        float z1 = rnd(current_prd.seed);
        float z2 = rnd(current_prd.seed);
        float3 light_pos = light.corner + light.v1 * z1 + light.v2 * z2;

        float Ldist = length(light_pos - hitpoint);
        float3 L = normalize(light_pos - hitpoint);
        float nDl = dot( world_shading_normal, L );
        float LnDl = dot( light.normal, L );
        float A = length(cross(light.v1, light.v2));

        // cast shadow ray
        if ( nDl > 0.0f && LnDl > 0.0f ) {
            PerRayData_pathtrace_shadow shadow_prd;
            shadow_prd.inShadow = false;
            shadow_prd.type = shadowRay;
            Ray shadow_ray = make_Ray( hitpoint, L, pathtrace_shadow_ray_type, scene_epsilon, Ldist );
            rtTrace(top_object, shadow_ray, shadow_prd);

            if(!shadow_prd.inShadow){
                float weight=nDl * LnDl * A / (M_PIf*Ldist*Ldist);
                result += light.emission * weight;
            }
        }
    }
    current_prd.radiance = result;
}
//-----------------------------------------------------------------------------
rtDeclareVariable(float3,        emission_color, , );

RT_PROGRAM void diffuseEmitter(){
    current_prd.radiance = current_prd.countEmitted? emission_color : make_float3(0.f);
    current_prd.done = true;
}
//-----------------------------------------------------------------------------
//
//  Exception program
//
//-----------------------------------------------------------------------------
RT_PROGRAM void exception(){
    output_buffer[launch_index] = make_float4(bad_color, 0.0f);

}
//-----------------------------------------------------------------------------
//
//  Miss program
//
//-----------------------------------------------------------------------------
RT_PROGRAM void miss(){
    current_prd.radiance = bg_color;
    current_prd.done = true;
}
//-----------------------------------------------------------------------------
RT_PROGRAM void envi_miss(){
    float theta = atan2f(ray.direction.x, ray.direction.z);
    float phi = M_PIf * 0.5f - acos(ray.direction.y);
    float u = (theta + M_PIf) * (0.5f * M_1_PIf);
    float v = 0.5f * ( 1.0f + sin(phi));
    current_prd.radiance = make_float3(tex2D(envmap, u, v));
    current_prd.done = true;
}
//-----------------------------------------------------------------------------
rtDeclareVariable(PerRayData_pathtrace_shadow, current_prd_shadow, rtPayload, );

RT_PROGRAM void shadow(){
    current_prd_shadow.inShadow = true;
    rtTerminateRay();
}
//-----------------------------------------------------------------------------
// OSL device function
__device__ int raytype(rayType _name){
    if (current_prd.type == _name){
        return 1;
    }
    else {
        return 0;
    }
}
//-----------------------------------------------------------------------------
__device__ optix::float3 text(char* _filename, float _s, float _t){
    return make_float3(tex2D(normalMap, _s, _t));
}
//----------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------- BRDFS -------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//__device__ optix::float3 reflection(optix::float3 _normal, float _eta){
//    current_prd.direction = reflect(ray.direction, _normal);
//    float cosNO = optix::dot(-_normal, eye - current_prd.origin);
//    if (cosNO > 0){
//        return optix::make_float3(fresnel_dielectric(cosNO, _eta));
//    }
//    return optix::make_float3(1.0);
//}
////----------------------------------------------------------------------------------------------------------------------
//__device__ optix::float3 diffuse(optix::float3 _normal){
//    float z1=rnd(current_prd.seed);
//    float z2=rnd(current_prd.seed);
//    float3 p;
//    cosine_sample_hemisphere(z1, z2, p);
//    float3 v1, v2;
//    createONB(_normal, v1, v2);
//    current_prd.direction = v1 * p.x + v2 * p.y + _normal * p.z;
//    return optix::make_float3(max(optix::dot(_normal, current_prd.direction), 0.0f) * float(M_1_PI));
//}
////----------------------------------------------------------------------------------------------------------------------
//__device__ optix::float3 phong(optix::float3 _normal, float _exponant){
//    float z1 = rnd(current_prd.seed);
//    float z2 = rnd(current_prd.seed);
//    float sinTheta = sqrt(1 - pow(z1, 2 / (_exponant + 1)));
//    float cosTheta= pow(z1, 1 / (_exponant + 1));
//    float sinPhi = sin(2*float(M_PI)*z2);
//    float cosPhi = cos(2*float(M_PI)*z2);
//    current_prd.direction = make_float3 (sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);

//    // n + 2 / 2PI * cos^n (A);
//    float3 R = reflect((current_prd.origin - eye), _normal);
//    float A = optix::dot(R, current_prd.direction);

//    if (A > 0){
//        return make_float3((( _exponant + 2) / (2 * M_PI * pow(A, _exponant))));
//    }
//    return make_float3(0.0);
//}
////----------------------------------------------------------------------------------------------------------------------
//__device__ optix::float3 oren_nayar(float3 _normal, float _sigma){
//    float z1=rnd(current_prd.seed);
//    float z2=rnd(current_prd.seed);
//    float3 p;
//    cosine_sample_hemisphere(z1, z2, p);
//    float3 v1, v2;
//    createONB(_normal, v1, v2);
//    float3 ray_direction = current_prd.direction;
//    current_prd.direction = v1 * p.x + v2 * p.y + _normal * p.z;

//    float3 L = current_prd.direction;
//    float3 V = (current_prd.origin - eye);

//    float A = 1 / (M_PI + ((M_PI / 2) - (2/3)) * _sigma );
//    float B = _sigma / (M_PI + ( (M_PI /2) - (2/3)) * _sigma);
//    float s = optix::dot(L, V) - optix::dot(_normal, L) * optix::dot(_normal, V);
//    float t;
//    if ( s <= 0.0){
//        t = 1.0;
//    }
//    else{
//        t = max(optix::dot(_normal, L), optix::dot(_normal, V));
//    }

//    return make_float3(0.8 * optix::dot(_normal, L) * (A + B * (s / t)));
//}
////----------------------------------------------------------------------------------------------------------------------
//__device__ optix::float3 ward(float3 _normal, float3 _t, float _xRough, float _yRough){
//    float3 direction = current_prd.direction;
//    float z1 = rnd(current_prd.seed);
//    float z2 = rnd(current_prd.seed);
//    float3 v = cross(_normal, _t);
//    float3 u = cross(v, _normal);

//    float phi = atan((_yRough/_xRough) * tan(2*float(M_PI)*z2));
//    float theta = atan( sqrt( -log(1 - z1) / ( ((cos(phi)*cos(phi)) / (_xRough*_xRough)) + ((sin(phi)*sin(phi)) / (_yRough*_yRough)) ) ) );
//    float3 h = make_float3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
//    h = h.x * u + h.y * v + h.z * _normal;
//    float3 o = 2 * dot(direction, h) * h + direction;
//    float ps = 1.0;
//    float w = ps * dot(h, direction)* pow(dot(h, _normal), 3) * sqrt(dot(o, _normal) / dot(direction, _normal));
//    current_prd.direction = o;

//    return make_float3((ps / ( 4 * float(M_PI) * _xRough * _yRough * sqrt(dot(current_prd.direction, _normal) * dot(direction, _normal)))) * exp(-( ((h.x / _xRough)*(h.x / _xRough)) + ((h.y / _yRough)*(h.y/_yRough)) / (dot(h,_normal)*dot(h,_normal))) ));
//}

//__device__ optix::float3 microfacet(char* _distribution, float3 _normal, float3 _u, float _xAlpha, float _yAlpha, float _eta, int _refract){
//    if (_distribution == "beckmann" || _distribution == "Beckmann"){
//        if (!_refract){
//            // Generate two random numbers
//            float z1 = rnd(current_prd.seed);
//            float z2 = rnd(current_prd.seed);

//            // Generate our microsurface normal m
//            float3 m;
//            float a2 = _xAlpha * _xAlpha;
//            float theta = atan(sqrt(-a2 * log(1 - z1)));
//            float phi = 2 * M_PI * z2;
//            m = optix::normalize(make_float3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)));
////            printf("m.x: %f, m.y: %f, m.z: %f\n", m.x, m.y, m.z);

//            float3 direction = current_prd.direction;

//            // Set the new ray direction
//            current_prd.direction = 2 * optix::dot(m, current_prd.origin - eye) * m - (current_prd.origin - eye);
//            float chi;
//            if (optix::dot(m, _normal) <= 0.0){
//                chi = 0.0;
//            }
//            else{
//                chi = 1.0;
//            }

//            // Calculate the fresnel term
//            float F = fresnel_dielectric(optix::dot(m, (current_prd.origin - eye)), _eta);

//            // Calculate the distribution
//            float D;
//            float mLen, nLen;
//            mLen = sqrt((m.x*m.x) + (m.y*m.y) + (m.z*m.z));
//            nLen = sqrt((_normal.x*_normal.x) + (_normal.y*_normal.y) + (_normal.z*_normal.z));
//            float thetaM = optix::dot(m, _normal) / (mLen*nLen);
//            D  = chi / (float(M_PI) * -a2 * pow(cos(thetaM), 4));
//            D *= exp(-(tan(thetaM)*tan(thetaM)) / a2);
////            printf("%f\n", D);

//            // Calculate the geometric distribution
//            float G1 = beckmannDistibution(direction, m, _normal, _xAlpha);
//            float G2 = beckmannDistibution(current_prd.direction, m, _normal, _xAlpha);
//            float G = G1 * G2;

//            return make_float3((F * G * D) / (4 * optix::dot(current_prd.origin - eye, _normal) * optix::dot(current_prd.direction, _normal)));
////            return make_float3(D);
//        }
//    }
//}

//__device__ float beckmannDistibution(float3 _v, float3 _m, float3 _normal, float _a){
//    float chi;
//    if (optix::dot(_v, _m) / optix::dot(_v, _normal) <= 0.0){
//        chi = 0.0;
//    }
//    else{
//        chi = 1.0;
//    }
//    float vLen = sqrt((_v.x*_v.x) + (_v.y*_v.y) + (_v.z*_v.z));
//    float nLen = sqrt((_normal.x*_normal.x) + (_normal.y*_normal.y) + (_normal.z*_normal.z));
//    float thetaV = optix::dot(_v, _normal) / (vLen * nLen);
//    float a = 1.0 / ( _a * tan(thetaV));//(optix::dot(_normal, (current_prd.origin - eye)) /( _a * sqrt(1 - (optix::dot(_normal, (current_prd.origin - eye))* optix::dot(_normal, (current_prd.origin - eye))))));
//    float G1;
//    // Supposedly cheaper
//    if (a < 1.6){
//        G1 = chi * ((3.535*a + 2.18*a*a) / (1 + 2.276*a + 2.577*a*a));
//    }
//    else{
//        G1 = chi;
//    }

//    return G1;
//}

