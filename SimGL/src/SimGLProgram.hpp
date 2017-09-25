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

// 着色器中一直变量。
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

typedef Vector<UniformRefrence> UniformReferenceList;

class GLProgram
{
public:
    GLProgram();
    ~GLProgram();
    
    void attach(GLShader* shader);
    
    GLuint getProgram() { return _program; }
    
    void active();
    bool isActive();
    
    void bindUniforms();
    
    void updateUniforms();
    
private:
    void _extractUniforms(const ShaderConstantDefinitionMap* verConstantDefs,
                          const ShaderConstantDefinitionMap* fragConstantDefs);

    void _compileAndLink();
    
private:
    GLuint _program;
    GLShader* _verShader;
    GLShader* _fragShader;
    
    bool _linked;
    
    bool _uniformRefsBuilt;
    
    UniformReferenceList _uniformRefs;
};

#endif /* SimGLProgram_hpp */
