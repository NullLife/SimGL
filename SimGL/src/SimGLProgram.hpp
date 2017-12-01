//
//  SimGLProgram.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimGLProgram_hpp
#define SimGLProgram_hpp

#include "SimCommon.hpp"
#include "SimGLShader.hpp"
#include "SimGLShaderParams.hpp"

/// 着色器中一直变量。
struct UniformRefrence
{
    // Uniform location.
    int _location;
    
    // Uniform's name.
    String _uniformName;
    
    // The type of shader.
    GLShaderType _shaderType;
    
    // Constant definition.
    const ShaderConstantDefinition* _constantDef;
};

/// 子程序
struct SubroutineUniformRefrence
{
    /// Uniform location.
    int _location;
    
    /// Uniform's name.
    String _uniformName;
    
    /// The type of shader.
    GLShaderType _shaderType;
    
    /// Constant definition.
    const ShaderConstantDefinition* _constantDef;
};

typedef Vector<UniformRefrence> UniformReferenceList;
typedef Vector<SubroutineUniformRefrence> SubroutineUniformReferenceList;
typedef HashMap<String, GLuint> SubroutineIndices;

class GLProgram
{
public:
    GLProgram();
    ~GLProgram();
    
    void attachVertexShader(GLShader* verShader);
    void attachGeometryShader(GLShader* geoShader);
    void attachFragmentShader(GLShader* fragShader);
    
    GLuint getProgram() { return _program; }
    
    void active();
    bool isActive();
    
    void bindUniforms();
    
    void updateUniforms();
    
    /// Get all subroutines.
    const SubroutineIndices& getSubroutineIndices() const { return _subroutineIndices; }
    
private:
    void _extractUniforms();
    void _extractSubroutines();

    void _compileAndLink();
    
private:
    GLuint _program;
    GLShader* _verShader;
    GLShader* _geoShader;
    GLShader* _fragShader;
    
    bool _linked;
    
    bool _uniformRefsBuilt;
    bool _subroutineUniformRefsBuilt;
    
    UniformReferenceList _uniformRefs;
    SubroutineUniformReferenceList _subroutineUniformRefs;
    SubroutineIndices _subroutineIndices;
};

#endif /* SimGLProgram_hpp */
