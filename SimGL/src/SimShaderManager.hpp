//
// Created by yangbin on 2017/4/26.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SHADERMANAGER_HPP
#define SIMGL_SHADERMANAGER_HPP

#include "SimSingleton.hpp"
#include "SimCommon.hpp"

class GLSLShader;

class ShaderManager : public Singleton<ShaderManager> {
public:
    typedef HashMap<String, GLShaderPtr> GLShaderCache;
public:
    ShaderManager();
    ~ShaderManager();
    
private:
    GLShaderCache mCache;
    
public:
    GLShaderPtr createShader(const String& name);
    
    GLShaderPtr getShader(const String& name);
};




#endif //SIMGL_SHADERMANAGER_HPP
