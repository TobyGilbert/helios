#include "Core/mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/AbstractMaterialWidget.h"
#include "Core/MaterialLibrary.h"
#include "UI/MeshWidget.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QGLFormat format;
    format.setVersion(4,1);
    format.setProfile(QGLFormat::CoreProfile);

    m_openGLWidget = new OpenGLWidget(format,this);
    ui->gridLayout->addWidget(m_openGLWidget,0,1,2,2);

    connect(m_openGLWidget,SIGNAL(pathTracerCreated()),this,SLOT(createMenus()));

}

MainWindow::~MainWindow(){

    //delete our singlton classes
    AbstractMaterialWidget::getInstance()->destroy();
    MaterialLibrary::getInstance()->destroy();
    MeshWidget::getInstance()->destroy();
    LightManager::getInstance()->destroy();
    delete ui;
    delete m_openGLWidget;
}

void MainWindow::createMenus(){
    //--------------------------------------------------------------------------------------------------------------------
    //----------------------------Create our node graph widget instance---------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------

    //init our instance with this as the parent. this means when this class is deleted it also will be deleted
    AbstractMaterialWidget::getInstance(this)->hide();
    //init our materail library
    MaterialLibrary::getInstance(this)->hide();
    //init our mesh library
    MeshWidget::getInstance(this)->hide();

    // A toolbar used to hold the button associated with different elements in the scene e.g. lighting, mesh options
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setOrientation(Qt::Vertical);
    ui->gridLayout->addWidget(toolBar, 0, 0, 2, 1);


    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Light functionality------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    QPixmap light("icons/light.png");
    QIcon lightBtnIcon(light);
    QToolButton *lightToolbarBtn = new QToolButton(toolBar);
    lightToolbarBtn->setIcon(lightBtnIcon);
    lightToolbarBtn->setToolTip("Light options");
    toolBar->addWidget(lightToolbarBtn);
    toolBar->addSeparator();

    LightManager::getInstance()->setHidden(true);
    this->addDockWidget(Qt::RightDockWidgetArea, LightManager::getInstance());
    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Mesh functionality-------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    // Button to access the mesh specific parameters
    QPixmap mesh("icons/teapot.png");
    QIcon meshBtnIcon(mesh);
    QToolButton *meshToolbarButton = new QToolButton(toolBar);
    meshToolbarButton->setIcon(meshBtnIcon);
    meshToolbarButton->setToolTip("Mesh options");
    toolBar->addWidget(meshToolbarButton);
    toolBar->addSeparator();

    this->addDockWidget(Qt::RightDockWidgetArea, MeshWidget::getInstance());
    connect(MeshWidget::getInstance(),SIGNAL(updateScene()),m_openGLWidget,SLOT(sceneChanged()));
    connect(meshToolbarButton, SIGNAL(clicked()), MeshWidget::getInstance(), SLOT(show()));

    //--------------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------Environment map----------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    // Set up toolbar button
    QPixmap environ("icons/environment.png");
    QIcon environBtnIcon(environ);
    QToolButton *environmentToolbarButton = new QToolButton(toolBar);
    environmentToolbarButton->setIcon(environBtnIcon);
    environmentToolbarButton->setToolTip("Environment Map Options");
    toolBar->addWidget(environmentToolbarButton);
    toolBar->addSeparator();

    // Set up widget
    QDockWidget *environmentDockWidget = new QDockWidget(this);
    environmentDockWidget->setWindowTitle("Environment Map Attributes");
    environmentDockWidget->setHidden(true);
    QGroupBox *environmentGroupBox = new QGroupBox(environmentDockWidget);
    environmentGroupBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    environmentDockWidget->setWidget(environmentGroupBox);
    QGridLayout *environmentGridLayout = new QGridLayout(environmentGroupBox);
    environmentGroupBox->setLayout(environmentGridLayout);
    m_environmentLineEdit = new QLineEdit(environmentGroupBox);
    environmentGridLayout->addWidget(m_environmentLineEdit, 0, 0, 1, 1);
    QPushButton *environmentButton = new QPushButton("Load",environmentGroupBox);
    environmentGridLayout->addWidget(environmentButton, 0, 1, 1, 1);
    this->addDockWidget(Qt::RightDockWidgetArea, environmentDockWidget);

    //--------------------------------------------------------------------------------------------------------------------
    //--------------------------------------------------Connections-------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------
    connect(lightToolbarBtn, SIGNAL(clicked(bool)), lightToolbarBtn, SLOT(setChecked(bool)));
//    connect(lightToolbarBtn, SIGNAL(clicked()), m_lightDockWidget, SLOT(show()));
//    connect(m_lightColourButton, SIGNAL(clicked()), m_lightColourDialog, SLOT(show()));
    connect(lightToolbarBtn, SIGNAL(clicked()), LightManager::getInstance(), SLOT(show()));


    connect(meshToolbarButton, SIGNAL(clicked(bool)), meshToolbarButton,  SLOT(setChecked(bool)));

    connect(environmentToolbarButton, SIGNAL(clicked(bool)), environmentToolbarButton, SLOT(setChecked(bool)));
    connect(environmentToolbarButton, SIGNAL(clicked()), environmentDockWidget, SLOT(show()));
    connect(environmentButton, SIGNAL(clicked()), m_openGLWidget, SLOT(loadEnvironmentMap()));
    connect(environmentButton, SIGNAL(clicked()), this, SLOT(displayEnvironmentMap()));

    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *fileMenu = new QMenu("File",menuBar);
    QAction *importAction = new QAction(tr("&Import"),fileMenu);
    connect(importAction,SIGNAL(triggered()),MeshWidget::getInstance(),SLOT(importModel()));
    fileMenu->addAction(importAction);
    fileMenu->addAction("Save");
    menuBar->addAction(fileMenu->menuAction());

    QMenu *renderMenu = new QMenu("Render",menuBar);

    QAction *saveImage = new QAction(tr("&Image"), renderMenu);
//    m_saveImage->setShortcut(QKeySequence::Image);
    saveImage->setStatusTip(tr("Render to image"));
    connect(saveImage, SIGNAL(triggered()), m_openGLWidget, SLOT(saveImage()));

    renderMenu->addAction(saveImage);
    menuBar->addAction(renderMenu->menuAction());

    //add our settings button on our toolbar
    QMenu *settingsMenu = new QMenu("Settings",menuBar);
    menuBar->addAction(settingsMenu->menuAction());
    QAction *generalSettings = new QAction(tr("&General Settings"),this);
    generalSettings->setStatusTip(tr("Change general settings of Helios"));
    settingsMenu->addAction(generalSettings);

    // create our general settings widget
    GenSetDockWidget *genSetwdg = new GenSetDockWidget(this);
    genSetwdg->setHidden(true);

    connect(generalSettings, SIGNAL(triggered()), genSetwdg, SLOT(show()));
    connect(genSetwdg, SIGNAL(signalMoveRenderReduction(int)),m_openGLWidget,SLOT(setMoveRenderReduction(int)));
    connect(genSetwdg, SIGNAL(signalSetTimeOutDur(int)),m_openGLWidget,SLOT(setTimeOutDur(int)));

}

void MainWindow::displayEnvironmentMap(){
    m_environmentLineEdit->setText(m_openGLWidget->getEnvironmentMap());
}
