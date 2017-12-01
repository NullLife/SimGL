//
// Created by yangbin on 2017/4/26.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimShaderManager.hpp"
#include "SimGLShader.hpp"

template<> ShaderManager* ShaderManager::Singleton<ShaderManager>::mSingleton = nullptr;
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
    LogManager::getSingleton().debug("delete ShaderManager");
    mCache.clear();
}


GLShaderPtr ShaderManager::createShader(const String& name)
{
    GLShaderPtr ptr(new GLShader(name));
    mCache.insert(GLShaderCache::value_type(name, ptr));
    return ptr;
}

GLShaderPtr ShaderManager::getShader(const String& name)
{
    auto iter = mCache.find(name);
    if (iter == mCache.end())
    {
        //LogManager::getSingleton().debug("ShaderManager::getShader", "Can't find the shader : "+ name + "in cache, please create it!");
        return nullptr;
    }
    return iter->second;
}


