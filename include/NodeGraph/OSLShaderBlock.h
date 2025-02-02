#ifndef OSLSHADERBLOCK_H
#define OSLSHADERBLOCK_H

//------------------------------------------------------------------------------------------------------------------------------------
/// @class OSLShaderBlock
/// @brief This is a node for our user interface specialised for loading in and setting up OSL shaders.
/// @author Declan Russell
/// @date 20/03/2014
//------------------------------------------------------------------------------------------------------------------------------------

#include <QGraphicsPathItem>
#include "NodeGraph/qneblock.h"

class OSLShaderBlock : public QNEBlock
{
public:
    enum { Type = QGraphicsItem::UserType + 4 };
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief our default constructor, takes in a path to osl shader and sets up our ports accordingly
    /// @param _path - path to our shader program
    /// @param parrent - parent of our class, this is optional.
    //------------------------------------------------------------------------------------------------------------------------------------
    OSLShaderBlock(QGraphicsItem *parent = 0);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to the this shaders cuda kernal
    /// @return Cuda kernal in the form of a string from of compiled OSL shader
    //------------------------------------------------------------------------------------------------------------------------------------
    inline std::string getDevicefunction(){return m_cudaKernal;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief accessor to the shader name
    /// @return shader name in the form of a string
    //------------------------------------------------------------------------------------------------------------------------------------
    inline std::string getBlockName(){return m_shaderName;}
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief compiles our osl shader and sets up our ports and obtains required data about shader
    /// @param _path - path to our shader file
    //------------------------------------------------------------------------------------------------------------------------------------
    bool loadShader(QString _path);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief returns the type of our class
    //------------------------------------------------------------------------------------------------------------------------------------
    int type() const { return Type; }
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overide our save function to give our new implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void save(QDataStream &ds);
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief overide our load function to give our new implimentation
    //------------------------------------------------------------------------------------------------------------------------------------
    void load(QDataStream &ds, QMap<quint64, QNEPort *> &portMap);
    //------------------------------------------------------------------------------------------------------------------------------------
private:
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief string to store the cuda kernal form of our osl shader
    //------------------------------------------------------------------------------------------------------------------------------------
    std::string m_cudaKernal;
    //------------------------------------------------------------------------------------------------------------------------------------
    /// @brief the name of our shader
    //------------------------------------------------------------------------------------------------------------------------------------
    std::string m_shaderName;
    //------------------------------------------------------------------------------------------------------------------------------------
};

#endif // OSLSHADERBLOCK_H
