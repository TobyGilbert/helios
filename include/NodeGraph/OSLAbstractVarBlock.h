#ifndef OSLABSTRACTVARBLOCK_H
#define OSLABSTRACTVARBLOCK_H

//------------------------------------------------------------------------------------------------------------------------------------
/// @class ColorNodeProxyWidget
/// @brief Extension of QNEblock, This Abstract class provides a basis for all variable blocks to be drawn in our scene
/// @author Declan Russell
/// @date 05/05/2015
//------------------------------------------------------------------------------------------------------------------------------------

#include "NodeGraph/qneblock.h"
#include <optixu/optixpp_namespace.h>
#include <QDoubleSpinBox>
#include <QGraphicsScene>
#include "AbstractNodeProxyWidget.h"

class   OSLAbstractVarBlock : public QNEBlock
{
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief this means you can define a new QGraphicsItem type. Woudln't of done it like this myself but lets go along with Stanislaw Adaszewski's implementation.
    enum { Type = QGraphicsItem::UserType + 5 };
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    OSLAbstractVarBlock(QGraphicsScene *_scene,optix::Material &_mat,QGraphicsItem * parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default destuctor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~OSLAbstractVarBlock(){}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Set the name of our variable block
    /// @param _name - desired name
    //------------------------------------------------------------------------------------------------------------------------------------
    QNEPort *setBlockName(std::string _name);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overiden virtual function to return the block name
    //------------------------------------------------------------------------------------------------------------------------------------
    inline std::string getBlockName(){return m_blockName;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief manualy set whatever our block variable is linked to to
    //------------------------------------------------------------------------------------------------------------------------------------
    inline void setLinkedVar(){m_widgetProxy->setLinkedVar();}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief returns the type of our class
    //------------------------------------------------------------------------------------------------------------------------------------
    int type() const { return Type; }
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief a member to hold our widget proxy
    //------------------------------------------------------------------------------------------------------------------------------------
    AbstractNodeProxyWidget *m_widgetProxy;
    //------------------------------------------------------------------------------------------------------------------------------------
protected:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief the material which out variable belongs to
    //------------------------------------------------------------------------------------------------------------------------------------
    optix::Material m_material;
    //------------------------------------------------------------------------------------------------------------------------------------


};


#endif // OSLABSTRACTVARBLOCK_H
