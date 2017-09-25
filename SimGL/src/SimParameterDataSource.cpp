//
//  FMKParameterDataSource.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/31.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParameterDataSource.hpp"

#include "SimCamera.hpp"
#include "SimPass.hpp"

ParameterDataSource::ParameterDataSource() :
    _modelMatrixDirty(true),
    _viewMatrixDirty(true),
    _projectionMatrixDirty(true),
    _mvMatrixDirty(true),
    _mvpMatrixDirty(true)
{
}

ParameterDataSource::~ParameterDataSource()
{
}

void ParameterDataSource::setModelMatrix(const Mat4& m)
{
    _modelMatrix = m;
    _modelMatrixDirty = true;
}

void ParameterDataSource::setAmbientLightColor(const Vec4& color)
{
    _ambientLightColor = color;
}

void ParameterDataSource::setCurrentCamera(Camera* cam)
{
    _currCamera = cam;
}

void ParameterDataSource::setLights(const LightList& lights)
{
    _lights = lights;
}

void ParameterDataSource::setCurrentPass(Pass* pass)
{
    _currPass = pass;
}

const Mat4& ParameterDataSource::getModelMatrix()
{
    return _modelMatrix;
}
const Mat4& ParameterDataSource::getViewMatrix()
{
    _viewMatrix = _currCamera->getViewMatrix();
    return _viewMatrix;
}
const Mat4& ParameterDataSource::getProjectionMatrix()
{
    _projectionMatrix = _currCamera->getProjectiveMatrix();
    return _projectionMatrix;
}

const Mat4& ParameterDataSource::getModelViewMatrix()
{
    _mvMatrix = getViewMatrix() * getModelMatrix();
    return _mvMatrix;
}
const Mat4& ParameterDataSource::getModelViewProjectionMatrix()
{
    _mvpMatrix = getProjectionMatrix() * getModelViewMatrix();
    return _mvpMatrix;
}

const Vec3& ParameterDataSource::getCameraPosition()
{
    return _currCamera->getPosition();
}

const Vec4& ParameterDataSource::getAmbientLightColor()
{
    return _ambientLightColor;
}

// ================= Light ================== //

const int ParameterDataSource::getLightCount()
{
    return (int) _lights.size();
}

const int ParameterDataSource::getLightType(int index)
{
    return _lights[index]->getType();
}

const Vec3& ParameterDataSource::getLightPosition(int index)
{
    return _lights[index]->getPosition();
}
const Vec3& ParameterDataSource::getLightDirection(int index)
{
    return _lights[index]->getDirection();
}

const Vec4& ParameterDataSource::getLightDiffuseColor(int index)
{
    return _lights[index]->getDiffuseColor();
}
const Vec4& ParameterDataSource::getLightSpecularColor(int index)
{
    return _lights[index]->getSpecularColor();
}

const float ParameterDataSource::getSpotLightCutoff(int index)
{
    return _lights[index]->getCutOff();
}

const float ParameterDataSource::getLightIntensity(int index)
{
    return _lights[index]->getIntensity();
}
const Vec4& ParameterDataSource::getLightAttenuation(int index)
{
    return _lights[index]->getAttenuation();
}

// ================= Material =====================//
const Vec4& ParameterDataSource::getSurfaceAmbient()
{
    return _currPass->getAmbient();
}

const Vec4& ParameterDataSource::getSurfaceDiffuse()
{
    return _currPass->getDiffuse();
}

const Vec4& ParameterDataSource::getSurfaceSpecular()
{
    return _currPass->getSpecular();
}

const Vec4& ParameterDataSource::getSurfaceEmission()
{
    return _currPass->getEmission();
}

const float ParameterDataSource::getSurfaceShininess()
{
    return _currPass->getShininess();
}



