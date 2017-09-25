//
// Created by yangbin on 2017/2/28.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimPass.hpp"
#include "SimResourceManager.hpp"
#include "SimTexture.hpp"
#include "SimShaderManager.hpp"
#include "SimTextureUnitState.hpp"
#include "SimGLShader.hpp"
#include "SimGLShaderParams.hpp"

Pass::Pass() :
    _name(""),
    _parent(nullptr),
    _tag(""),
    _lightEnable(false),
    _verShaderName(""),
    _fragShaderName(""),
    _verShader(nullptr),
    _fragShader(nullptr)
{
}

Pass::Pass(const String &name) :
    _name(name),
    _parent(nullptr),
    _tag(""),
    _lightEnable(false),
    _verShaderName(""),
    _fragShaderName(""),
    _verShader(nullptr),
    _fragShader(nullptr)
{
}

Pass::~Pass()
{
    TextureUnitStateList::iterator iter = _texStateList.begin();
    for (; iter != _texStateList.end(); ++iter)
    {
        delete *iter;
        _texStateList.erase(iter);
    }
    
    _texStateList.shrink_to_fit();
    
    LogManager::getSingleton().debug("Delete Pass name", _name);
}

void Pass::setVertexShaderName(const String& shaderName)
{
    _verShaderName = shaderName;
}

void Pass::setFragmentShaderName(const String& shaderName)
{
    _fragShaderName = shaderName;
}

TextureUnitState* Pass::createTextureUnitState()
{
    TextureUnitState* texUnitState = new TextureUnitState(this);
    addTextureUnitState(texUnitState);
    return texUnitState;
}


int Pass::addTextureUnitState(TextureUnitState *texState)
{
    size_t index = _texStateList.size();
    _texStateList.push_back(texState);
    return static_cast<int>(index);
}

void Pass::addTextureUnitStates(TextureUnitState** texStates, int texStateNum)
{
    for (int i=0; i<texStateNum; ++i)
    {
        _texStateList.push_back(texStates[i]);
    }
}

GLShader* Pass::getVertexShader() {
    if (_verShader)
        return _verShader;
    
    if (_verShaderName == "")
        return nullptr;
    
    GLShaderPtr shader = ShaderManager::getSingleton().getShader(_verShaderName);
    if (shader == nullptr)
        return nullptr;
    
    _verShader = shader.get();
    
    return _verShader;
}

GLShader* Pass::getFragmentShader()
{
    if (_fragShader)
        return _fragShader;
    
    if (_fragShaderName == "")
        return nullptr;
    
    GLShaderPtr shader = ShaderManager::getSingleton().getShader(_fragShaderName);
    if (!shader)
        return nullptr;
    
    _fragShader = shader.get();
    
    return _fragShader;
}

void Pass::updateParameters(ParameterDataSource* dataSource)
{
    if (_verShader)
    {
        _verShader->getParameters()->_updateParameters(dataSource);
    }
    
    if (_fragShader)
    {
        _fragShader->getParameters()->_updateParameters(dataSource);
    }
}

void Pass::setAmbient(const Vec4& ambient)
{
    _ambient = ambient;
}

const Vec4& Pass::getAmbient() const
{
    return _ambient;
}

void Pass::setDiffuse(const Vec4& diffuse)
{
    _diffuse = diffuse;
}

const Vec4& Pass::getDiffuse() const
{
    return _diffuse;
}

void Pass::setSpecular(const Vec4& specular)
{
    _specular = specular;
}

const Vec4& Pass::getSpecular() const
{
    return _specular;
}

void Pass::setEmission(const Vec4& emission)
{
    _emission = emission;
}

const Vec4& Pass::getEmission() const
{
    return _emission;
}

void Pass::setShininess(float shininess)
{
    _shininess = shininess;
}

float Pass::getShininess() const
{
    return _shininess;
}

