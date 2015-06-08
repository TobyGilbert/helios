#ifndef LIGHTMANAGER_H_
#define LIGHTMANAGER_H_
//----------------------------------------------------------------------------------------------------------------------
/// @brief A class to manage all the lights in our scene
/// @author Toby Gilbert
/// @date 30/05/15
//----------------------------------------------------------------------------------------------------------------------
#include <QDockWidget>
#include <QWidget>
#include <vector>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
//----------------------------------------------------------------------------------------------------------------------
// QT GUI includes
//----------------------------------------------------------------------------------------------------------------------
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpacerItem>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QListWidget>
#include "Lights/Light.h"


class LightManager : public QDockWidget{
    Q_OBJECT
public:
    struct LightTransforms{
        glm::vec3 m_translate;
        glm::vec3 m_scale;
        glm::vec3 m_rotate;
    };
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns an instance of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static LightManager *getInstance(QWidget *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~LightManager();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Initialise the light buffer
    //----------------------------------------------------------------------------------------------------------------------
    void initialise();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief destroys our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    inline void destroy(){delete m_instance;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Create a new parallelogram light
    //----------------------------------------------------------------------------------------------------------------------
    void createParollelogramLight();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns our optix buffer containing information about our lights
    //----------------------------------------------------------------------------------------------------------------------
    inline optix::Buffer getLightsBuffer(){return m_lightBuffer;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the transform node for attaching to the scene graph
    //----------------------------------------------------------------------------------------------------------------------
    inline std::vector<optix::Transform> getGeomAndTrans(){return m_geoAndTrans;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Used for translating lights
    //----------------------------------------------------------------------------------------------------------------------
    void setTrans(optix::Transform _transform, glm::mat4 _trans, bool _transpose=0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sets the GUI to the default values
    //----------------------------------------------------------------------------------------------------------------------
    void setGuiDefaults();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Transforms lights when scene camera moves
    //----------------------------------------------------------------------------------------------------------------------
    void transformLights(glm::mat4 _trans);
    //----------------------------------------------------------------------------------------------------------------------
signals:
    void updateScene();
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //----------------------------------------------------------------------------------------------------------------------
    explicit LightManager(QWidget *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a pointer to our instance of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static LightManager* m_instance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Creates our GUI elements
    //----------------------------------------------------------------------------------------------------------------------
    void createGUI();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A buffer to hold our lights
    //----------------------------------------------------------------------------------------------------------------------
    optix::Buffer m_lightBuffer;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A variable to store the number of lights in our buffer
    //----------------------------------------------------------------------------------------------------------------------
    unsigned int m_numLights;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A list to hold all our lights
    //----------------------------------------------------------------------------------------------------------------------
    QListWidget* m_lightIndexListWidget;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our translation gui box for the x axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_translateX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our translation gui box for the y axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_translateY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our translation gui box for the z axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_translateZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our scale gui box for the x axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_scaleX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our scale gui box for the y axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_scaleY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our scale gui box for the z axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_scaleZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our rotation gui box for the x axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_rotateX;
    //---------------------------------------------------------------------------------------------------------------------
    /// @brief Our rotation gui box for the y axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_rotateY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our rotation gui box for the z axis
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_rotateZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our emission gui box for the red componant
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_emissionX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our emission gui box for the green componant
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_emissionY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our emission gui box for the blue componant
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_emissionZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The currently seleted item
    //----------------------------------------------------------------------------------------------------------------------
    int m_selectedLight;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A vector to hold pointers to all the lights in our scene
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<Light*> m_lights;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief A vector to store parallelogram light structs for copying to the GPU
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<Light::ParallelogramLight> m_parallelogramLights;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Light transforms
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<optix::Transform> m_geoAndTrans;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Light transform struct for use with GUI
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<LightTransforms> m_lightTransforms;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The global translation matrix
    //----------------------------------------------------------------------------------------------------------------------
    glm::mat4 m_transGlobal;
    //----------------------------------------------------------------------------------------------------------------------
public slots:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Adds a light to the scene
    //----------------------------------------------------------------------------------------------------------------------
    void addLight();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Update a light already created
    //----------------------------------------------------------------------------------------------------------------------
    void updateLight();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Update the gui for the current light selected
    //----------------------------------------------------------------------------------------------------------------------
    void updateGUI(QModelIndex _index);
    //----------------------------------------------------------------------------------------------------------------------
};

#endif
