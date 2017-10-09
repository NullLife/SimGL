//
//  SimGLSLShaderParams.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimGLShaderParams.hpp"

#include "SimGLProgram.hpp"
#include "SimParameterDataSource.hpp"

GLShaderParams::GLShaderParams()
{
    _namedConstants = ShaderNamedConstantsPtr(new ShaderNamedConstants);
}

GLShaderParams::~GLShaderParams()
{
    _floatConstants.clear();
    _intConstants.clear();
    _doubleConstants.clear();
}

void GLShaderParams::addConstantDefinition(const String& name, ShaderConstantType scType, ShaderConstantContent scc, size_t arraySize)
{
    if (_namedConstants->_defMap.find(name) != _namedConstants->_defMap.end())
    {
        LogManager::getSingleton().debug("GpuSharedParameters::addConstantDefinition",
                                         "Constant entry with name '" + name + "' already exists. ");
        return;
    }
    
    ShaderConstantDefinition def;
    def._arraySize = arraySize;
    def._type = scType;
    def._content = scc;
    def._elementComponentSize = ShaderConstantDefinition::getElementComponentSize(scType);
    
    if (def.isFloat())
    {
        def._physicalIndex = _floatConstants.size();
        _floatConstants.resize(_floatConstants.size() + def._arraySize * def._elementComponentSize);
    }
    else if (def.isDouble())
    {
        def._physicalIndex = _doubleConstants.size();
        _doubleConstants.resize(_doubleConstants.size() + def._arraySize * def._elementComponentSize);
    }
    else
    {
        def._physicalIndex = _intConstants.size();
        _intConstants.resize(_intConstants.size() + def._arraySize * def._elementComponentSize);
    }
    
    _namedConstants->_defMap[name] = def;
}

ShaderConstantDefinition* GLShaderParams::findConstantDefinition(const std::string& name, bool throwErrorIfNotFound)
{
    if (_namedConstants == nullptr)
    {
        if (throwErrorIfNotFound)
            LogManager::getSingleton().error("FMKShaderParameters::findNamedConstantDefinition",
                                             "Named constants have not been initialised.");
        return nullptr;
    }
    
    ShaderConstantDefinitionMap::iterator it =  _namedConstants->_defMap.find(name);
    if (it != _namedConstants->_defMap.end())
        return &(it->second);
    
    if (throwErrorIfNotFound)
        LogManager::getSingleton().error("FMKShaderParameters::findNamedConstantDefinition",
                                         "Parameter called " + name + " does not exist. ");

    return nullptr;
}

void GLShaderParams::_updateParameters(ParameterDataSource* dataSource)
{
    ShaderConstantDefinitionMap::const_iterator iter = _namedConstants->_defMap.begin();
    ShaderConstantDefinitionMap::const_iterator iterEnd = _namedConstants->_defMap.end();
    for (; iter!=iterEnd; ++iter)
    {
        const ShaderConstantDefinition& def = iter->second;
        switch (def._content)
        {
            case SCC_MODEL_MATRIX:
                _writeRawData(def._physicalIndex, &dataSource->getModelMatrix()[0][0], def._elementComponentSize);
                break;
                
            case SCC_VIEW_MATRIX:
                _writeRawData(def._physicalIndex, &dataSource->getViewMatrix()[0][0], def._elementComponentSize);
                break;
                
            case SCC_PROJECTIVE_MATRIX:
                _writeRawData(def._physicalIndex, &dataSource->getProjectionMatrix()[0][0], def._elementComponentSize);
                break;
                
            case SCC_MV_MATRIX:
                _writeRawData(def._physicalIndex, &dataSource->getModelViewMatrix()[0][0], def._elementComponentSize);
                break;
                
            case SCC_MVP_MATRIX:
                _writeRawData(def._physicalIndex, &dataSource->getModelViewProjectionMatrix()[0][0], def._elementComponentSize);
                break;
                
                // Ambient light
            case SCC_AMBIENT_LIGHT_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getAmbientLightColor()[0], def._elementComponentSize);
                break;
                
                // Light
            case SCC_LIGHT_COUNT:
                _writeRawData(def._physicalIndex, dataSource->getLightCount());
                break;
                
            case SCC_LIGHT_TYPE:
                _writeRawData(def._physicalIndex, dataSource->getLightType((int)def._data));
                break;
                
            case SCC_SPOT_LIGHT_CUTOFF:
                _writeRawData(def._physicalIndex, dataSource->getSpotLightCutoff((int)def._data));
                break;
                
            case SCC_LIGHT_INTENSITY:
                _writeRawData(def._physicalIndex, dataSource->getLightIntensity((int)def._data));
                break;
                
            case SCC_LIGHT_ATTENUATION:
                _writeRawData(def._physicalIndex,& dataSource->getLightAttenuation((int)def._data)[0], def._elementComponentSize);
                break;
                
            case SCC_LIGHTPOSITION_WORLD_SPACE:
                _writeRawData(def._physicalIndex, &dataSource->getLightPosition((int)def._data)[0], def._elementComponentSize);
                break;
                
            case SCC_LIGHTDIRECTION_WORLD_SPACE:
                _writeRawData(def._physicalIndex, &dataSource->getLightDirection((int)def._data)[0], def._elementComponentSize);
                break;
                
            case SCC_LIGHT_DIFFUSE_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getLightDiffuseColor((int)def._data)[0], def._elementComponentSize);
                break;
                
            case SCC_LIGHT_SPECULAR_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getLightSpecularColor((int)def._data)[0], def._elementComponentSize);
                break;
                
                // Material
            case SCC_SURFACE_AMBIENT_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getSurfaceAmbient()[0], def._elementComponentSize);
                break;
            case SCC_SURFACE_DIFFUSE_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getSurfaceDiffuse()[0], def._elementComponentSize);
                break;
            case SCC_SURFACE_SPECULAR_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getSurfaceSpecular()[0], def._elementComponentSize);
                break;
            case SCC_SURFACE_EMISSIVE_COLOR:
                _writeRawData(def._physicalIndex, &dataSource->getSurfaceEmission()[0], def._elementComponentSize);
                break;
            case SCC_SURFACE_SHININESS:
                _writeRawData(def._physicalIndex, dataSource->getSurfaceShininess());
                break;
                
            case SCC_CAMERAPOSITION_WORLD_SPACE:
                _writeRawData(def._physicalIndex, &dataSource->getCameraPosition()[0], def._elementComponentSize);
                break;
              
            // Other
            case SCC_TIME:
                _writeRawData(def._physicalIndex, dataSource->getTime());
                break;
        }
    }
}



void GLShaderParams::setNamedConstant(const String& name, int val)
{
    setNamedConstant(name, &val, 1);
}

void GLShaderParams::setNamedConstant(const String& name, float val)
{
    setNamedConstant(name, &val, 1);
}

void GLShaderParams::setNamedConstant(const String& name, double val)
{
    setNamedConstant(name, &val, 1);
}

void GLShaderParams::setNamedConstant(const String& name, Vec2 val)
{
    setNamedConstant(name, &val[0], 2);
}

void GLShaderParams::setNamedConstant(const String& name, Vec3 val)
{
    setNamedConstant(name, &val[0], 3);
}

void GLShaderParams::setNamedConstant(const String& name, Vec4 val)
{
    setNamedConstant(name, &val[0], 4);
}

void GLShaderParams::setNamedConstant(const String& name, Mat4 val)
{
    setNamedConstant(name, &val[0][0], 16);
}

void GLShaderParams::setNamedConstant(const String& name, const float *val, size_t count)
{
    ShaderConstantDefinition* def = findConstantDefinition(name);
    if (def)
        _writeRawData(def->_physicalIndex, val, count);
}

void GLShaderParams::setNamedConstant(const String& name, const double *val, size_t count)
{
    ShaderConstantDefinition* def = findConstantDefinition(name);
    if (def)
        _writeRawData(def->_physicalIndex, val, count);
}

void GLShaderParams::setNamedConstant(const String& name, const int *val, size_t count)
{
    ShaderConstantDefinition* def = findConstantDefinition(name);
    if (def)
        _writeRawData(def->_physicalIndex, val, count);
}

void GLShaderParams::_writeRawData(size_t index, const int val)
{
    _writeRawData(index, &val, 1);
}

void GLShaderParams::_writeRawData(size_t index, const float val)
{
    _writeRawData(index, &val, 1);
}

void GLShaderParams::_writeRawData(size_t index, const double val)
{
    _writeRawData(index, &val, 1);
}

void GLShaderParams::_writeRawData(size_t index, const int* val, size_t count)
{
    if (index + count > _intConstants.size())
        LogManager::getSingleton().error("GLShaderParams::_writeRawData", "Out of bound.");
    
    memcpy(&_intConstants[index], val, sizeof(int) * count);
}

void GLShaderParams::_writeRawData(size_t index, const float* val, size_t count)
{
    if (index + count > _floatConstants.size())
        LogManager::getSingleton().error("GLShaderParams::_writeRawData", "Out of bound.");

    memcpy(&_floatConstants[index], val, sizeof(int) * count);
}

void GLShaderParams::_writeRawData(size_t index, const double* val, size_t count)
{
    if (index + count > _doubleConstants.size())
        LogManager::getSingleton().error("GLShaderParams::_writeRawData", "Out of bound.");
    
    memcpy(&_doubleConstants[index], val, sizeof(int) * count);
}

float* GLShaderParams::getFloatPointer(size_t pos)
{
    return &_floatConstants[pos];
}

int* GLShaderParams::getIntPointer(size_t pos)
{
    return &_intConstants[pos];
}

double* GLShaderParams::getDoublePointer(size_t pos)
{
    return &_doubleConstants[pos];
}
