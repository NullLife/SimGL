//
//  Camera.cpp
//  SimGL
//
//  Created by yangbin on 2017/4/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimCamera.hpp"

Camera::Camera(const std::string& name) :
    Frustum(),
    _name(name),
    _needUpdateView(false),
    _minDist(6.0f),
    _maxDist(64.0f),
    _camPos(Vec3(0.0f, 3.0f, 1.0f)),
    _camTarget(Vec3(0.0f, 0.0f, 0.0f)),
    _camUp(Vec3(0.0f, 1.0f, 0.0f)),
    _camRight(Vec3(1.0f, 0.0f, 0.0f)),
    _orientation(1.0f, 0.0f, 0.0f, 0.0f),
    _scale(1.0f)
{
    setPosition(_camPos);
    _viewMatrix = glm::lookAt(_camPos, _camPos + _camLook, _camUp);
}

Camera::~Camera()
{
}

const std::string& Camera::getName()
{
    return _name;
}

void Camera::setPosition(const Vec3 &pos)
{
    // Distance.
    _camLook = _camTarget - pos;
    _curDist = glm::length(_camLook);
    
    _camLook /= _curDist;
    
    _curDist = std::fmax(_minDist, std::fmin(_curDist, _maxDist));
    
    // Calcuate position of camera and reset it.
    _camPos = -_camLook * _curDist;
    
    _needUpdateView =  true;
}

const Vec3& Camera::getPosition()
{
    _updateView();
    return _camPos;
}

void Camera::setTarget(const Vec3& target)
{
    _camTarget = target;
    
    // Distance.
    _camLook = _camTarget - _camPos;
    _curDist = glm::length(_camLook);
    
    _camLook /= _curDist;
    
    _curDist = std::fmax(_minDist, std::fmin(_curDist, _maxDist));
    
    // Calcuate position of camera and reset it.
    _camPos = -_camLook * _curDist;
    
    _needUpdateView = true;
}

const Vec3 Camera::getTarget()
{
    return _camTarget;
}

const Vec3 Camera::getUp()
{
    _updateView();
    return _camUp;
}

const Vec3 Camera::getLook()
{
    _updateView();
    return _camLook;
}

const Mat4& Camera::getViewMatrix()
{
    _updateView();
    return _viewMatrix;
}

float Camera::getPitch()
{
    _updateView();
    return _pitch;
}

float Camera::getYaw()
{
    _updateView();
    return _yaw;
}

float Camera::getRoll()
{
    _updateView();
    return _roll;
}

//------------------ Transform ----------------------//

void Camera::rotate(Quat quat)
{
    _orientation = quat * _orientation;
    _orientation = glm::normalize(_orientation);
    
    _needUpdateView = true;
}


void Camera::rotate(Vec3 axis, float rad)
{
    axis = glm::normalize(axis);
    Quat q(rad, axis);
    q = glm::normalize(q);
    rotate(q);
}

void Camera::pitch(float rad)
{
    // x
    rotate(X_AXIS, -rad);
}

void Camera::yaw(float rad)
{
    // y
    rotate(Y_AXIS, -rad);
}

void Camera::roll(float rad)
{
    // z
    rotate(Z_AXIS, -rad);
}

void Camera::translate(float dx, float dy, float dz)
{
    _updateView();
    
    Vec3 x = _camRight * dx;
    Vec3 y = _camUp * dy;
    Vec3 z = _camLook * dz;
    
    _camPos += (x+y+z);
    _camTarget += (x+y+z);
    
    _camLook = _camTarget - _camPos;
    _curDist = glm::length(_camLook);
    _camLook /= _curDist;
    _curDist = std::fmax(_minDist, std::fmin(_curDist, _maxDist));
    
    _needUpdateView = true;
}

void Camera::scale(float scale)
{
    _updateView();
    
    _scale = scale;
    
    LogManager::getSingleton().debug("Camera", "scale-->" + std::to_string(_scale));
    
    // Scale is applied to distance.
    _curDist /= scale;
    
    _curDist = std::fmax(_minDist, std::fmin(_curDist, _maxDist));
    
    _camPos = -_camLook * _curDist;
    
    _needUpdateView = true;
}

//------------------- update ------------------------//
void Camera::_updateView()
{
    if (!_needUpdateView) {
        return;
    }
    
    // Rotate
    Mat3 rotM;
    rotM = glm::mat3_cast(_orientation);
    
    Vec3 T = rotM * (-_camLook);
    T = T * _curDist;
    _camPos = _camTarget + T;
    
    _camLook = _camTarget - _camPos;
    _camLook = glm::normalize(_camLook);
    
    _camUp = rotM * _camUp;
    _camUp = glm::normalize(_camUp);
    
    _camRight = glm::cross(_camLook, _camUp);
    _camRight = glm::normalize(_camRight);
    
    _viewMatrix = glm::lookAt(_camPos, _camPos + _camLook, _camUp);
    
    // Returns euler angles, yitch as x, yaw as y, roll as z.
    Vec3 euler = glm::eulerAngles(_orientation);
    _pitch = euler.x;
    _yaw = euler.y;
    _roll = euler.z;
    
    LogManager::getSingleton().debug("Camera", "pos-->" + toString(_camPos));
    LogManager::getSingleton().debug("Camera", "target-->" + toString(_camTarget));
//    LogManager::getSingleton().debug("Camera", "look-->" + toString(_camLook));
//    LogManager::getSingleton().debug("Camera", "right-->" + toString(_camRight));
//    LogManager::getSingleton().debug("Camera", "up-->" + toString(_camUp));
    
//    LogManager::getSingleton().debug("Camera", "dist-->" + std::to_string(_curDist));
    
//    LogManager::getSingleton().debug("Camera", "orien-->" + toString(Vec3f(RadiansToDegrees(_pitch),
//                                                                              RadiansToDegrees(_yaw),
//                                                                              RadiansToDegrees(_roll))));
    
    _orientation = Quat(1.0f, 0.0f, 0.0f, 0.0f);
    _needUpdateView = false;
}

void Camera::_updateFrustum()
{
    Frustum::updateFrustum();
}

std::string Camera::toString(const Vec3& val)
{
    return "x: "+std::to_string(val.x) + ",y: "+std::to_string(val.y) + ",z: " + std::to_string(val.z);
}

