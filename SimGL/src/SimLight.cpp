//
//  SimLight.cpp
//  SimGL
//
//  Created by yangbin on 2017/5/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimLight.hpp"

Light::Light(const LightType type) :
    _type(type),
    _intensity(1.0),
    _diffuseColor(Vec4(1.0, 1.0, 1.0, 1.0)),
    _specularColor(Vec4(1.0, 1.0, 1.0, 1.0)),
    _position(Vec3(0, 0, 0)),
    _cutoff(0.5),
    _direction(Vec3(1, 1, 1)),
    _atten(Vec4(1.0, 1.0, 0.09, 0.032))
{
}

Light::~Light()
{
    LogManager::getSingleton().debug("Delete Light");
}

const LightType Light::getType() const
{
    return _type;
}

void Light::setIntensity(float intensity)
{
    _intensity = intensity;
}
const float Light::getIntensity() const
{
    return _intensity;
}

void Light::setDiffuseColor(const Vec4& color)
{
    _diffuseColor = color;
}

const Vec4& Light::getDiffuseColor() const
{
    return _diffuseColor;
}

void Light::setSpecularColor(const Vec4& color)
{
    _specularColor = color;
}

const Vec4& Light::getSpecularColor() const
{
    return _specularColor;
}

void Light::setDirection(const Vec3& direction)
{
    _direction = direction;
}

const Vec3& Light::getDirection() const
{
    return _direction;
}

void Light::setPosition(const Vec3& pos)
{
    _position = pos;
}

const Vec3& Light::getPosition() const
{
    return _position;
}

void Light::setCutOff(float rad)
{
    _cutoff = rad;
}

const float Light::getCutOff() const
{
    return _cutoff;
}

void Light::setAttenuation(float range, float constant, float linear, float quadratic)
{
    _atten = Vec4(constant, linear, quadratic, range);
}

const Vec4& Light::getAttenuation() const
{
    return _atten;
}

const float Light::getAttenuationRange() const
{
    return _atten.w;
}

const float Light::getAttenuationConstant() const
{
    return _atten.x;
}

const float Light::getAttenuationLinear() const
{
    return _atten.y;
}

const float Light::getAttenuationQuadric() const
{
    return _atten.z;
}



