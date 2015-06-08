#ifndef OSLVARCOLORBLOCK_H
#define OSLVARCOLORBLOCK_H

//------------------------------------------------------------------------------------------------------------------------------------
/// @class OSLVarFloatBlock
/// @brief This class is used for creating a color variable node in our node graphics interface.
/// @author Declan Russell
/// @date 21/03/2015
//------------------------------------------------------------------------------------------------------------------------------------

#include "NodeGraph/OSLAbstractVarBlock.h"
#include <optixu/optixpp_namespace.h>
#include <QGraphicsScene>

class OSLVarColorBlock : public OSLAbstractVarBlock
{
public:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default constructor
    //------------------------------------------------------------------------------------------------------------------------------------
    OSLVarColorBlock(QGraphicsScene *_scene,optix::Material _mat,QGraphicsItem * parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief default destructor
    //------------------------------------------------------------------------------------------------------------------------------------
    ~OSLVarColorBlock(){}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our save function for color node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void save(QDataStream &ds);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overload our load function for color node implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void load(QDataStream &, QMap<quint64, QNEPort *> &portMap);
    //------------------------------------------------------------------------------------------------------------------------------------

};

#endif // OSLVARCOLORBLOCK_H
