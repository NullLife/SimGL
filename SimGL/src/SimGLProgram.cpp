//
//  SimGLProgram.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimGLProgram.hpp"

GLProgram::GLProgram() :
    _program(0),
    _verShader(nullptr),
    _fragShader(nullptr),
    _linked(false),
    _uniformRefsBuilt(false)
{
}

GLProgram::~GLProgram()
{
    if (_program > 0)
    {
        glDeleteProgram(_program);
        _program = 0;
    }
}

void GLProgram::attach(GLShader* shader)
{
    if (shader == nullptr)
        return;
    
    switch (shader->getType())
    {
        case GST_VERTEX:
            _verShader = shader;
            break;
        
        case GST_FRAGMENT:
            _fragShader = shader;
            break;
            
        default:
            break;
    }
    
    _linked = false;
    _uniformRefsBuilt = false;
}

void GLProgram::active()
{
    if (!_linked)
    {
        _compileAndLink();
    }
    // The program was already active, return.
    if (isActive())
    {
        return;
    }
    
    glUseProgram(_program);
}

bool GLProgram::isActive()
{
    if (!_linked || _program == 0)
    {
        return false;
    }
    
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return currentProgram == _program;
}

void GLProgram::_compileAndLink()
{
    // Create the program object
    _program = glCreateProgram();
    if (_program == 0)
        LogManager::getSingletonPtr()->error("GLProgram::_link", "create program failed.");
    
    // Compile all the shaders and Attach to program.
    if (!_verShader || !_fragShader)
        LogManager::getSingletonPtr()->error("GLProgram::_link", "Must hava vertex shader and fragment shader.");
    
    if (_verShader->_compile())
       glAttachShader(_program, _verShader->getId());
    
    if (_fragShader->_compile())
        glAttachShader(_program, _fragShader->getId());
    
    // Link the shaders together
    glLinkProgram(_program);

    // Detach all the shaders
    glDetachShader(_program, _verShader->getId());
    glDetachShader(_program, _fragShader->getId());

    // Throw exception if linking failed
    GLint status = -1;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        String msg = "Program linking failure: ";
        
        GLint infoLogLength;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(_program, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(_program);
        _program = 0;
        LogManager::getSingletonPtr()->error("GLProgram::_link", msg);
        
        _linked = false;
        return;
    }
    
    _linked = true;
    
    // Bind uniforms
    bindUniforms();
}

void GLProgram::bindUniforms()
{
    if (!_uniformRefsBuilt)
    {
        _extractUniforms(&_verShader->getParameters()->getNamedConstants()->_defMap,
                         &_fragShader->getParameters()->getNamedConstants()->_defMap);
        
        _uniformRefsBuilt = true;
    }
}

void GLProgram::_extractUniforms(const ShaderConstantDefinitionMap* verConstantDefs,
                                 const ShaderConstantDefinitionMap* fragConstantDefs)
{
    if (!_linked || _program == 0)
    {
        return;
    }
    
    GLint uniformCount = 0;
    #define BUF_SIZE 200
    GLchar uniformName[BUF_SIZE] = "";
    GLint size;
    GLenum type;
    GLint location;
    
    // Get actived uniform's count in program.
    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &uniformCount);
    
    UniformRefrence newUniformRef;
    ShaderConstantDefinitionMap::const_iterator iter;
    
    for (int i=0; i<uniformCount; ++i)
    {
        glGetActiveUniform(_program, i, BUF_SIZE, NULL, &size, &type, uniformName);
        
        // Get the uniform location.
        location = glGetUniformLocation(_program, uniformName);
        
        if (location >= 0)
        {
            // Record.
            std::string name(uniformName);
            
            // Found out if it in vertex constants.
            iter = verConstantDefs->find(name);
            if (iter != verConstantDefs->end())
            {
                newUniformRef._location = location;
                newUniformRef._uniformName = name;
                newUniformRef._constantDef = &(iter->second);
                
                newUniformRef._shaderType = GLShaderType::GST_VERTEX;
                
                _uniformRefs.push_back(newUniformRef);
                
                continue;
            }
            
            // Found out if it in fragment constants.
            iter = fragConstantDefs->find(name);
            if (iter != fragConstantDefs->end())
            {
                newUniformRef._location = location;
                newUniformRef._uniformName = name;
                newUniformRef._constantDef = &(iter->second);
                
                newUniformRef._shaderType = GLShaderType::GST_FRAGMENT;
                
                _uniformRefs.push_back(newUniformRef);
            }
        }
        
    }
}

void GLProgram::updateUniforms()
{
    UniformReferenceList::iterator currentUniform = _uniformRefs.begin();
    for (; currentUniform != _uniformRefs.end(); ++currentUniform) {
        const ShaderConstantDefinition* def = currentUniform->_constantDef;
        GLsizei glArraySize = (GLsizei)def->_arraySize;
        
        // If the matrix need to transpose. colum major is false.
        bool transpose = GL_FALSE;
        
        GLShaderParams* params;
        if (currentUniform->_shaderType == GST_VERTEX)
        {
            params = _verShader->getParameters().get();
        }
        else
        {
            params = _fragShader->getParameters().get();
        }
        
        switch (def->_type) {
            case SCT_FLOAT1:
                glUniform1fv(currentUniform->_location, glArraySize, params->getFloatPointer(def->_physicalIndex));
                break;
            case SCT_FLOAT2:
                glUniform2fv(currentUniform->_location, glArraySize, params->getFloatPointer(def->_physicalIndex));
                break;
            case SCT_FLOAT3:
                glUniform3fv(currentUniform->_location, glArraySize, params->getFloatPointer(def->_physicalIndex));
                break;
            case SCT_FLOAT4:
                glUniform4fv(currentUniform->_location, glArraySize, params->getFloatPointer(def->_physicalIndex));
                break;
                
            case SCT_MATRIX_3X3:
                glUniformMatrix3fv(currentUniform->_location, glArraySize, transpose, params->getFloatPointer(def->_physicalIndex));
                break;
            case SCT_MATRIX_4X4:
                glUniformMatrix4fv(currentUniform->_location, glArraySize, transpose, params->getFloatPointer(def->_physicalIndex));
                break;
                
            case SCT_INT1:
                glUniform1iv(currentUniform->_location, glArraySize, (GLint*)params->getIntPointer(def->_physicalIndex));
                break;
            case SCT_INT2:
                glUniform2iv(currentUniform->_location, glArraySize, (GLint*)params->getIntPointer(def->_physicalIndex));
                break;
            case SCT_INT3:
                glUniform3iv(currentUniform->_location, glArraySize, (GLint*)params->getIntPointer(def->_physicalIndex));
                break;
            case SCT_INT4:
                glUniform4iv(currentUniform->_location, glArraySize, (GLint*)params->getIntPointer(def->_physicalIndex));
                break;
                
            case SCT_SAMPLER1D:
            case SCT_SAMPLER2D:
            case SCT_SAMPLER3D:
            case SCT_SAMPLERCUBE:
                // samplers handled like 1-element ints
                glUniform1iv(currentUniform->_location, 1, (GLint*)params->getIntPointer(def->_physicalIndex));
                break;
            case SCT_UNKNOWN:
            default:
                break;
        }
        
    }
}
