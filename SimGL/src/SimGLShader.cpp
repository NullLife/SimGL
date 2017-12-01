//
//  SimGLShader.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimGLShader.hpp"
#include "SimGLShaderParams.hpp"

#include <fstream>
#include <sstream>

GLShader::GLShader(const String& name) :
    _type(GST_VERTEX),
    _name(name),
    _compiled(false),
    _id(0),
    _language(""),
    _verison(""),
    _feedbackMode(TFM_SEPARATE),
    _shaderParams(nullptr)
{
}

GLShader::~GLShader()
{
    LogManager::getSingleton().debug("delete shader: " + _name);
    if (_id > 0)
        glDeleteShader(_id);
}


const GLShaderParamsPtr& GLShader::getParameters()
{
    if (_shaderParams == nullptr)
    {
        _shaderParams = GLShaderParamsPtr(new GLShaderParams());
    }
    return _shaderParams;
}

void GLShader::setTransformFeedbackMode(const TransformFeedbackMode mode)
{
    _feedbackMode = mode;
}

void GLShader::addFeedbackVarying(const String varying)
{
    auto iter = _feedbackVaryings.begin();
    for (; iter != _feedbackVaryings.end(); ++iter)
    {
        if ((*iter) == varying)
            return;
    }
    
    _feedbackVaryings.push_back(varying);
}

void GLShader::_setTransformFeedbackVaryings(GLuint program)
{
    if (_feedbackVaryings.empty())
        return;
    
    Vector<const char *> feedbacks;
    feedbacks.resize(_feedbackVaryings.size());
    feedbacks.reserve(_feedbackVaryings.size());
    
    for (int i =0; i != _feedbackVaryings.size(); ++i)
        feedbacks[i] = _feedbackVaryings[i].c_str();
    
    SIM_CHECK_GL_ERROR( glTransformFeedbackVaryings(program,
                                                    (GLsizei) _feedbackVaryings.size(),
                                                    &feedbacks[0],
                                                    _feedbackMode)
                       );
    feedbacks.clear();
}

bool GLShader::_compile()
{
    if (_compiled)
        return true;

    String filepath  = SHADER_RESOURCE_DIR + _name;

    LogManager::getSingleton().debug("GLSLShader::_compile", "compiling shader: " + filepath);
    
    GLenum type;
    switch (_type)
    {
        case GST_VERTEX:
            type = GL_VERTEX_SHADER;
            break;
            
        case GST_GEOMETRY:
            type = GL_GEOMETRY_SHADER;
            break;

        case GST_FRAGMENT:
            type = GL_FRAGMENT_SHADER;
            break;
    }
    
    // Create the shader object
    GLuint shaderId = glCreateShader(type);
    if (shaderId == 0)
        LogManager::getSingletonPtr()->error("GLSLShader::_compile", "glCreateShader failed");
    
    // Open file
    std::ifstream f;
    f.open(filepath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
        LogManager::getSingletonPtr()->error("GLSLShader::_compile", "Failed to open file: " + filepath);
    
    // Read whole file into std::stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();
    
    // Set the source code
    String content = buffer.str();
    const char *code = content.c_str();
    glShaderSource(shaderId, 1, &code, NULL);
    // Compile
    glCompileShader(shaderId);
    
    // Throw exception if compile error occurred
    GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::string msg("Compile failure in " + filepath + ":\n");
        
        GLint infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(shaderId);
        shaderId = 0;
        
        LogManager::getSingletonPtr()->error("Shader::compileShader", msg);
    }
    
    _id = shaderId;
    
    return _compiled = true;
}


