#include "NodeGraph/OSLAbstractVarBlock.h"

OSLAbstractVarBlock::OSLAbstractVarBlock(QGraphicsScene *_scene,optix::Material &_mat,QGraphicsItem * parent) : QNEBlock(parent)
{
    // add this block to our scene
    _scene->addItem(this);
    //set our material
    m_material = _mat;
}
//------------------------------------------------------------------------------------------------------------------------------------
void OSLAbstractVarBlock::setBlockName(std::string _name){
    addPort(_name.c_str(),0,"",QNEPort::TypeVoid, QNEPort::TypePort);
}

//------------------------------------------------------------------------------------------------------------------------------------
