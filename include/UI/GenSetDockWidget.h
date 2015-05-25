#ifndef GENSETDOCKWIDGET_H
#define GENSETDOCKWIDGET_H

/// @class GenSetDockWiget
/// @date 26/02/2015
/// @author Declan Russell
/// @brief A Dockable widget which will hold controls to change the general settings of our application
/// @version 1.0

#include <QDockWidget>
#include <QGridLayout>
#include <QSpacerItem>
#include <QGroupBox>
#include <QSlider>
#include <QObject>
#include <QLabel>
#include "Core/pathtracerscene.h"
#include "Core/OpenGLWidget.h"

class GenSetDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor
    //----------------------------------------------------------------------------------------------------------------------
    explicit GenSetDockWidget(QWidget *parent = 0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the scene our general settings apply to
    //----------------------------------------------------------------------------------------------------------------------
    void installScene(OpenGLWidget* _scene);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our destructor
    //----------------------------------------------------------------------------------------------------------------------
    ~GenSetDockWidget();
    //----------------------------------------------------------------------------------------------------------------------
signals:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a signal to change the movement render reduction
    //----------------------------------------------------------------------------------------------------------------------
    void signalMoveRenderReduction(int _reductionAmount);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a signal to change our time our duration
    //----------------------------------------------------------------------------------------------------------------------
    void signalSetTimeOutDur(int _timeout);
    //----------------------------------------------------------------------------------------------------------------------
public slots:
    //----------------------------------------------------------------------------------------------------------------------
    ///  @brief slot to call change in movement render reduction
    //----------------------------------------------------------------------------------------------------------------------
    inline void moveRenderReductionSlot(int _reductionAmount){signalMoveRenderReduction(_reductionAmount);}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to call change in timeout
    //----------------------------------------------------------------------------------------------------------------------
    inline void setTimeOutDurSlot(int _timeout){signalSetTimeOutDur(_timeout);}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief slot to set the max ray depth
    /// @param _depth - desired ray depth
    //----------------------------------------------------------------------------------------------------------------------
    inline void setMaxRayDepth(int _depth){PathTracerScene::getInstance()->setMaxRayDepth(_depth);}
    //----------------------------------------------------------------------------------------------------------------------

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our openGL scene
    //----------------------------------------------------------------------------------------------------------------------
    OpenGLWidget *m_scene;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief our group box to hold our controls
    //----------------------------------------------------------------------------------------------------------------------
    QGroupBox *m_genSetGroupBox;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the layout of our group box widget
    //----------------------------------------------------------------------------------------------------------------------
    QGridLayout *m_layout;
    //----------------------------------------------------------------------------------------------------------------------

};

#endif // GENSETDOCKWIDGET_H
