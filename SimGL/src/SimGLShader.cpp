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
    mType(GST_VERTEX),
    mName(name),
    mCompiled(false),
    mId(0),
    mLanguage(""),
    mVerison(""),
    mShaderParams(nullptr)
{
}

GLShader::~GLShader()
{
    LogManager::getSingleton().debug("delete shader: " + mName);
    if (mId > 0)
        glDeleteShader(mId);
}


const GLShaderParamsPtr& GLShader::getParameters()
{
    if (mShaderParams == nullptr)
    {
        mShaderParams = GLShaderParamsPtr(new GLShaderParams());
    }
    return mShaderParams;
}

bool GLShader::_compile()
{
    if (mCompiled)
        return true;

    String filepath  = SHADER_RESOURCE_DIR + mName;

    LogManager::getSingleton().debug("GLSLShader::_compile", "compiling shader: " + filepath);
    
    GLenum type;
    switch (mType)
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
    
    mId = shaderId;
    
    return mCompiled = true;
}


