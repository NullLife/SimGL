//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimTransform.hpp"

Transform::Transform() :
    _needUpdateMatrix(true),
    _pos(Vec3(0.0f)),
    _scale(Vec3(1.0f)),
    _orientation(0, 0, 0, 1),
    _pitch(0), _yaw(0), _roll(0),
    _trans(Vec3(0, 0, 0)),
    _modelMatrix(Mat4(1.0f))
{
}

Transform::~Transform()
{
    LogManager::getSingleton().debug("delete Transform");
}

void Transform::setPosition(const Vec3& pos)
{
    _pos = pos;
    _needUpdateMatrix = true;
}

const Vec3 &Transform::getPosition()
{
    return _pos;
}

void Transform::translate(float dx, float dy, float dz)
{
    _trans = Vec3(dx, dy, dz);
    
    _needUpdateMatrix = true;
}


void Transform::pitch(float rad)
{
    rotate(X_AXIS, rad);
}

void Transform::yaw(float rad)
{
    rotate(Y_AXIS, rad);
}

void Transform::roll(float rad)
{
    rotate(Z_AXIS, rad);
}

void Transform::rotate (Quat quat)
{
    _orientation = quat * _orientation;
    _orientation = glm::normalize(_orientation);

    _needUpdateMatrix = true;
    
}

void Transform::rotate(Vec3 axis, float rad)
{
    axis = glm::normalize(axis);
    Quat q(rad, axis);
    q = glm::normalize(q);
    rotate(q);
}

void Transform::scale(const Vec3& scaleF)
{
    _scale = scaleF;
    _needUpdateMatrix = true;
}

const Mat4 &Transform::getModelMatrix()
{
    update();
    return _modelMatrix;
}

void Transform::update()
{
    if (!_needUpdateMatrix)
        return;

    // Translate
    Mat4 tranM = glm::translate(_modelMatrix, _trans);
    
    // Rotate
    Mat4 rotM = glm::mat4_cast(_orientation);
    
    // Scale
    Mat4 scaleM = glm::scale(_modelMatrix, _scale);
    
    _modelMatrix = tranM * rotM * scaleM;

    // Reset
    _scale = Vec3(1.0f);
    _orientation = Quat(0, 0, 0, 1);
    _trans = Vec3(0.0f);
    
    _needUpdateMatrix = false;
}
