//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimTransform.hpp"

Transform::Transform() :
    _needUpdateMatrix(true),
    _pos(Vec3(0.0f)),
    _scale(Vec3(1.0f)),
    _orientation(1.0f, 0.0f, 0.0f, 0.0f),
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
    _trans =_pos;
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
    _orientation = glm::rotate(_orientation, rad, X_AXIS);
    _needUpdateMatrix = true;
}

void Transform::yaw(float rad)
{
    _orientation = glm::rotate(_orientation, rad, Y_AXIS);
    _needUpdateMatrix = true;
}

void Transform::roll(float rad)
{
    _orientation = glm::rotate(_orientation, rad, Z_AXIS);
    _needUpdateMatrix = true;
}

void Transform::rotate (Quat quat)
{
    Vec3 axis = glm::axis(quat);
    axis = glm::normalize(axis);
    
    float rad = glm::angle(quat);
    
    _orientation = glm::rotate(_orientation, rad, axis);
    _needUpdateMatrix = true;
}

void Transform::rotate(Vec3 axis, float rad)
{
    axis = glm::normalize(axis);
    _orientation = glm::rotate(_orientation, rad, axis);
    _needUpdateMatrix = true;
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
    _modelMatrix = glm::translate(_modelMatrix, _trans);
    
    // Rotate
    _modelMatrix *= glm::mat4_cast(_orientation);
    
    // Scale
    _modelMatrix = glm::scale(_modelMatrix, _scale);

    // Reset
    _scale = Vec3(1.0f);
    _orientation = Quat(1, 0, 0, 0);
    _trans = Vec3(0.0f);
     
    _needUpdateMatrix = false;
}
