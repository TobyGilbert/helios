#ifndef PATHTRACERSCENE_H
#define PATHTRACERSCENE_H

#include <optixu/optixpp_namespace.h>
#include <QImage>

#include "random.h"
#include "path_tracer.h"
#include "helpers.h"


using namespace optix;

class PathTracerScene
{
protected:
    typedef optix::float3 float3;
    typedef optix::float4 float4;
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    //----------------------------------------------------------------------------------------------------------------------
    PathTracerScene();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief initialise our class
    //----------------------------------------------------------------------------------------------------------------------
    void init();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our trace function that launches our Optix context
    //----------------------------------------------------------------------------------------------------------------------
    void trace();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns our output buffer
    //----------------------------------------------------------------------------------------------------------------------
    inline optix::Buffer getOutputBuffer(){return m_context["output_buffer"]->getBuffer();}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the square root number of samples
    //----------------------------------------------------------------------------------------------------------------------
    inline void setNumSamples( unsigned int sns ){ m_sqrt_num_samples= sns; }
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a mutator to set the width and height of our scene
    //----------------------------------------------------------------------------------------------------------------------
    inline void setSize(unsigned int _width, unsigned int _height){m_width = _width; m_height = _height;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief resize our scene
    //----------------------------------------------------------------------------------------------------------------------
    void resize(int _width, int _height);
    //----------------------------------------------------------------------------------------------------------------------
protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief An instance of the optix engine
    //----------------------------------------------------------------------------------------------------------------------
    optix::Context m_context;
    //----------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a function to create the geomtry in our scene, probably not going to be around for long
    //----------------------------------------------------------------------------------------------------------------------
    void createGeometry();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns a geometry instance of a parralelogram
    //----------------------------------------------------------------------------------------------------------------------
    optix::GeometryInstance createParallelogram( const float3& anchor, const float3& offset1, const float3& offset2);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns an instance of a parralelogram with an emissive material
    //----------------------------------------------------------------------------------------------------------------------
    optix::GeometryInstance createLightParallelogram( const float3& anchor, const float3& offset1, const float3& offset2, int lgt_instance = -1);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief creates a material and adds it to our geomtry instance
    //----------------------------------------------------------------------------------------------------------------------
    void setMaterial( optix::GeometryInstance& gi, optix::Material material, const std::string& color_name, const float3& color);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our bounding box program
    /// @todo probably doesnt need to be a member but we'll get rid of that later
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_bounding_box;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our intersection program
    /// @todo probably doesn't need to be a member but we'll get rid of that later
    //----------------------------------------------------------------------------------------------------------------------
    optix::Program m_pgram_intersection;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the depth of our ray?
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_rr_begin_depth;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the sqrt of the number of samples we want
    /// @brief sqrt's are expensive GPU so lets do it on the CPU
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_sqrt_num_samples;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief current frame number
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_frame;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our sampling strategy, dont think that we need this
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_sampling_strategy;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the width of our scene
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the height of our scene
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_height;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief out output buffer
    //----------------------------------------------------------------------------------------------------------------------
    optix::Buffer m_outputBuffer;
    //----------------------------------------------------------------------------------------------------------------------

};

#endif // PATHTRACERSCENE_H
