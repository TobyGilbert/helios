#ifndef CAMERAWIDGET_H_
#define CAMERAWIDGET_H_
//----------------------------------------------------------------------------------------------------------------------
/// @class MeshWidget
/// @date 02/05/15
/// @author Toby Gilbert
/// @brief This class is an extention of QDockWidget that includes our camera controls
//----------------------------------------------------------------------------------------------------------------------
#include <QDockWidget>
#include <QLabel>
#include <QDoubleSpinBox>

//----------------------------------------------------------------------------------------------------------------------
class CameraWidget : public QDockWidget
{
    Q_OBJECT
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief returns the instance of our camera widget
    //----------------------------------------------------------------------------------------------------------------------
    static CameraWidget *getInstance(QWidget *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief destroys our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static void destroy();
    //----------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~CameraWidget();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    //----------------------------------------------------------------------------------------------------------------------
    explicit CameraWidget(QWidget *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief pointer to the instance of our singleton class
    //----------------------------------------------------------------------------------------------------------------------
    static CameraWidget* m_instance;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief "Aperture Radius"
    //----------------------------------------------------------------------------------------------------------------------
    QLabel* m_apertureLabel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The aperture radius of our camera
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_apertureRadiusSB;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief "Focal Point"
    //----------------------------------------------------------------------------------------------------------------------
    QLabel* m_focalPointLabel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief X componant of focal point
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_focalPointXSB;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Y componant of focal point
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_focalPointYSB;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Z componant of focal point
    //----------------------------------------------------------------------------------------------------------------------
    QDoubleSpinBox* m_focalPointZSB;
    //----------------------------------------------------------------------------------------------------------------------
public slots:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Enables the depth of field controls
    //----------------------------------------------------------------------------------------------------------------------
    void enableDOF(bool _enabled);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Update DOF camera settings
    //----------------------------------------------------------------------------------------------------------------------
    void updateDOF();
    //----------------------------------------------------------------------------------------------------------------------
};
//----------------------------------------------------------------------------------------------------------------------
#endif
