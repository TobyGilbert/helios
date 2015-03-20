/// @brief Class to interpret the oso code and turn it into a device function
/// @author Toby Gilbert
/// @date 04/03/15
#ifndef OSOREADER_H_
#define OSOREADER_H_
#include <string>
#include <vector>
#include <map>
#include <FlexLexer.h>
//class osoFlexLexer;
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief Symbols types for OSO files
//----------------------------------------------------------------------------------------------------------------------------------------
enum SymType {
    SymTypeParam, SymTypeOutputParam,
    SymTypeLocal, SymTypeTemp, SymTypeGlobal, SymTypeConst,
    SymTypeFunction, SymTypeType
};
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief Types used in OSL
//----------------------------------------------------------------------------------------------------------------------------------------
enum Type{
    TypeInt,TypeFloat,TypeString, TypeColour, TypeMatrix, TypeNormal, TypePoint, TypeVoid, TypeVector
};
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief An instruction within the OSO code section
//----------------------------------------------------------------------------------------------------------------------------------------
struct Instruction{
    std::string m_opcode = std::string("void");
    std::string m_output = std::string("void");
    std::vector<std::string> m_args;
};
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief A struction to represent any symbol used in a particular shader
//----------------------------------------------------------------------------------------------------------------------------------------
struct Symbol{
    int m_symType;
    Type m_type;
    std::string m_name;
    std::vector<std::string> m_initialParams;
};
//----------------------------------------------------------------------------------------------------------------------------------------
extern int yyparse ();
//----------------------------------------------------------------------------------------------------------------------------------------
class OsoReader{
public:
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor
    //----------------------------------------------------------------------------------------------------------------------------------------
    OsoReader();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor
    //----------------------------------------------------------------------------------------------------------------------------------------
    ~OsoReader();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Set the oso version
    //----------------------------------------------------------------------------------------------------------------------------------------
    void version(float _major, int _minor);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Create the shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    void shader(std::string _shaderType, std::string _shaderName);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Save the parameters passed to the shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    void parameter(std::pair<std::string,std::string> _params);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Add a symbols a vector storing all symbols used
    //----------------------------------------------------------------------------------------------------------------------------------------
    void addSymbols(int _symtype, Type _type, std::string _identifier);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Add the default parameters for a symbol
    //----------------------------------------------------------------------------------------------------------------------------------------
    void addSymbolDefaults(std::string _default);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief An instruction within the shader code
    //----------------------------------------------------------------------------------------------------------------------------------------
    void instruction(std::string _opcode);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief argument for the instructions
    //----------------------------------------------------------------------------------------------------------------------------------------
    void instructionArguments(std::string _argument);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Parse the oso file
    //----------------------------------------------------------------------------------------------------------------------------------------
    bool parseFile(const std::string &_filename);
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Print the osl shader version
    //----------------------------------------------------------------------------------------------------------------------------------------
    inline void printVersion(){std::cout<<"Open Shading Language Version: "<<m_osoMajor<<"."<<m_osoMinor<<std::endl;}
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Print the osl shader currently being parsed;
    //----------------------------------------------------------------------------------------------------------------------------------------
    inline void printShader(){std::cout<<"Shader: "<<m_shaderType<<" "<<m_shaderName<<std::endl;}
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Print the parameters input into the OSL shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    void printParams();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Print the instructions and its arguments
    //----------------------------------------------------------------------------------------------------------------------------------------
    void printInstructions();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Generate cuda device function
    //----------------------------------------------------------------------------------------------------------------------------------------
    void printDeviceFunction();
    std::string generateDeviceFunction();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the input parameters for the current OSL shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::vector<Symbol> getInputParams();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Returns the outpu parameters for the current OSL shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::vector<Symbol> getOutputParams();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Reset all vectors
    //----------------------------------------------------------------------------------------------------------------------------------------
    void resetVectors();
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief A vector containing all the symbols used in an OSL shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::vector<Symbol> m_symbols;
    //----------------------------------------------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Oso major version
    //----------------------------------------------------------------------------------------------------------------------------------------
    float m_osoMajor;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief Oso minor version
    //----------------------------------------------------------------------------------------------------------------------------------------
    int m_osoMinor;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The type of shader
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::string m_shaderType;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The shader name
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::string m_shaderName;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The shaders input parameters
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::vector<std::pair<std::string, std::string>> m_iParams;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The shaders instructions
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::vector<Instruction> m_instructions;
    //----------------------------------------------------------------------------------------------------------------------------------------
    /// @brief The functions the shader is using
    //----------------------------------------------------------------------------------------------------------------------------------------
    std::map<std::string, std::string> m_instructionFunctions;
    //----------------------------------------------------------------------------------------------------------------------------------------

};
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief Return the singleton instance of the the class OsoReader
//----------------------------------------------------------------------------------------------------------------------------------------
OsoReader* getOsoReader();
//----------------------------------------------------------------------------------------------------------------------------------------
/// @brief An instance of the class OsoReader used as a singleton
//----------------------------------------------------------------------------------------------------------------------------------------
static OsoReader *osoReader;
//----------------------------------------------------------------------------------------------------------------------------------------
#endif
