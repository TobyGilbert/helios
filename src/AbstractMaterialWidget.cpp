#include "AbstractMaterialWidget.h"
#include "pathtracerscene.h"

#include "qneport.h"
#include "OSLShaderBlock.h"
#include "OSLVarFloatBlock.h"
#include <QMenu>
#include <QPoint>
#include <QFileDialog>
#include <QPushButton>

AbstractMaterialWidget::AbstractMaterialWidget(QWidget *parent) :
    QWidget(parent)
{
    //set our widget layout
    m_widgetLayout = new QGridLayout(this);
    this->setLayout(m_widgetLayout);
    //add our groupbox
    m_widgetGroupBox = new QGroupBox(this);
    m_widgetLayout->addWidget(m_widgetGroupBox,0,0,1,1);
    m_groupBoxLayout = new QGridLayout(this);
    m_widgetGroupBox->setLayout(m_groupBoxLayout);
    //create our graphics view to hold our node interface scene
    m_graphicsView = new QGraphicsView(this);
    //add this widget to our group box
    m_groupBoxLayout->addWidget(m_graphicsView,0,0,1,1);
    //create our node interface scene
    m_nodeInterfaceScene = new QGraphicsScene(this);
    //set our graphics view scene to that of our node interface scene
    m_graphicsView->setScene(m_nodeInterfaceScene);
    //some stuff to make it look nicer
    m_graphicsView->setRenderHint(QPainter::Antialiasing, true);
    //create our node editor, could become useful in the future
    m_nodeEditor = new OSLNodesEditor(this);
    m_nodeEditor->install(m_nodeInterfaceScene);

    //add a button to to launch the creation of our shader
    QPushButton *createShaderBtn = new QPushButton("Create Shader",this);
    m_groupBoxLayout->addWidget(createShaderBtn,0,1,1,1);
    connect(createShaderBtn,SIGNAL(clicked()),m_nodeEditor,SLOT(createOptixMaterial()));

    //Set up our menu for if you right click in our widget
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showContextMenu(QPoint)));

    //create a material in our context
    m_material = PathTracerScene::getInstance()->getContext()->createMaterial();
}
//------------------------------------------------------------------------------------------------------------------------------------
AbstractMaterialWidget::~AbstractMaterialWidget(){
    //delete all the nodes that we have created
    for(unsigned int i=0; i<m_nodes.size();i++){
        delete m_nodes[i];
    }

}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::showContextMenu(const QPoint &pos){

    QPoint globalPos = this->mapToGlobal(pos);

    //our drop down menu
    QMenu myMenu;
    QAction *addShaderNodeBtn = new QAction(&myMenu);
    addShaderNodeBtn->setText("Add Shader Node");
    addShaderNodeBtn->setData(QVariant(0));
    myMenu.addAction(addShaderNodeBtn);

    QAction *addFloatNodeBtn = new QAction(&myMenu);
    addFloatNodeBtn->setText("Add Float Node");
    addFloatNodeBtn->setData(QVariant(1));
    myMenu.addAction(addFloatNodeBtn);

    //find out if something has been clicked
    QAction* selectedItem = myMenu.exec(globalPos);
    if(selectedItem){
        switch(selectedItem->data().toInt())
        {
            case(0): addShaderNode(); break;
            case(1): addFloatNode(); break;
            //if nothing do nothing
            default: break;
        }
    }


}

//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::addShaderNode()
{
    //let the user select a shader to load in
    QString location = QFileDialog::getOpenFileName(0,QString("Import Shader"), QString("shaders/"), QString("OSL files (*.osl)"));
    //if nothing selected then we dont want to do anything
    if(location.isEmpty()) return;

    //create a new shader node in our ui
    OSLShaderBlock *b = new OSLShaderBlock();
    //add it to out interface. This needs to be don before we add any
    //ports or it will not work, should probably do something about this
    m_nodeInterfaceScene->addItem(b);
    b->loadShader(location);
    //add it to our list of nodes
    m_nodes.push_back(b);
}
//------------------------------------------------------------------------------------------------------------------------------------
void AbstractMaterialWidget::addFloatNode()
{
    //create a float block
    OSLVarFloatBlock *b = new OSLVarFloatBlock();
    //add it to our interface scene
    m_nodeInterfaceScene->addItem(b);
    b->setValue();
    m_nodes.push_back(b);
}
//------------------------------------------------------------------------------------------------------------------------------------
