//
//  SimGLProgramManager.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimGLProgramManager.hpp"
#include "SimGLShader.hpp"
#include "SimGLProgram.hpp"

template<> GLProgramManager *Singleton<GLProgramManager>::mSingleton = nullptr;

GLProgramManager::GLProgramManager() :
    _activeProgram(nullptr),
    _verShader(nullptr),
    _geoShader(nullptr),
    _fragShader(nullptr)
{
}

GLProgramManager::~GLProgramManager()
{
    GLProgramCache::iterator iter = _cache.begin();
    for (; iter != _cache.end(); ++iter)
    {
        delete iter->second;
        _cache.erase(iter);
    }
}

void GLProgramManager::setActiveVertexShader(GLShader *verShader)
{
    if (_verShader != verShader)
    {
        _verShader = verShader;
        _activeProgram = nullptr;
        glUseProgram(0);
    }
}

void GLProgramManager::setActiveGeometryShader(GLShader *geoShader)
{
    if (_geoShader != geoShader)
    {
        _geoShader = geoShader;
        _activeProgram = nullptr;
        glUseProgram(0);
    }
}

void GLProgramManager::setActiveFragmentShader(GLShader *fragShader)
{
    if (_fragShader != fragShader)
    {
        _fragShader = fragShader;
        _activeProgram = nullptr;
        glUseProgram(0);
    }
}

GLProgram* GLProgramManager::getActiveProgram()
{
    if (_activeProgram)
    {
        return _activeProgram;
    }
    
    SimUInt64 activeKey = 0;
    if (_verShader)
    {
        _verShader->_compile();
        activeKey = static_cast<SimUInt64>(_verShader->getId()) << 32;
    }
    
    if (_geoShader)
    {
        _geoShader->_compile();
        activeKey += static_cast<SimUInt64>(_geoShader->getId()) << 16;
    }
    
    if (_fragShader)
    {
        _fragShader->_compile();
        activeKey += static_cast<SimUInt64>(_fragShader->getId());
    }
    
    if (activeKey > 0)
    {
        auto iter = _cache.find(activeKey);
        if (iter != _cache.end())
        {
            _activeProgram = iter->second;
        }
        else
        {
            _activeProgram = new GLProgram();
            _activeProgram->attachVertexShader(_verShader);
            _activeProgram->attachGeometryShader(_geoShader);
            _activeProgram->attachFragmentShader(_fragShader);
            
            _cache[activeKey] = _activeProgram;
        }
    }
    
    if (_activeProgram)
    {
        _activeProgram->active();
    }
    
    return _activeProgram;
}


