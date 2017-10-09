//
//  SimFrustum.cpp
//  SimGL
//
//  Created by yangbin on 2017/9/4.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimFrustum.hpp"

Frustum::Frustum() :
    _projType(PT_PERSPECTIVE),
    _aspectRatio(1.0f),
    _fovy(45.0f),
    _zNear(1.0f),
    _zFar(200.0f),
    _widowWidth(800.0f),
    _widowHeight(600.0f),
    _needUpdateFrustum(true)
{
}

Frustum::~Frustum()
{
}

void Frustum::setProjectionType(ProjectionType type)
{
    _projType = type;
}

ProjectionType Frustum::getProjectionType()
{
    return _projType;
}

void Frustum::fastInitFrustum(float fovy, float aspectRatio, float nearDist, float farDist)
{
    _fovy = fovy;
    _aspectRatio = aspectRatio;
    _zNear = nearDist;
    _zFar = farDist;
    
    updateFrustum();
}

// Angle
void Frustum::setFovy(float fovy)
{
    _fovy = fovy;
    
    updateFrustum();
}

float Frustum::getFovy()
{
    return _fovy;
}

void Frustum::setAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
    
    updateFrustum();
}

float Frustum::getAspectRatio()
{
    return _aspectRatio;
}

void Frustum::setNearClipDistance(float nearDist)
{
    _zNear = nearDist;
    
    updateFrustum();
}

float Frustum::getNearClipDistance()
{
    return _zNear;
}

void Frustum::setFarClipDistance(float farDist)
{
    _zFar = farDist;
    
    updateFrustum();
}

float Frustum::getFarClipDistance()
{
    return _zFar;
}

void Frustum::setOrthWindow(float w, float h)
{
    _widowWidth = w;
    _widowHeight = h;
    _aspectRatio = w / h;
    
    updateFrustum();
}
void Frustum::setFrustumExtends(float left, float right, float bottom, float top)
{
    _left = left;
    _top = top;
    _right = right;
    _bottom = bottom;
    
    updateFrustum();
}

const Mat4& Frustum::getProjectiveMatrix()
{
    _updateFrustumImp();
    return _projMatrix;
}


void Frustum::updateFrustum()
{
    _needUpdateFrustum = true;
}

void Frustum::_updateFrustumImp()
{
    if (!_needUpdateFrustum) {
        return;
    }
    
    if (_projType == PT_PERSPECTIVE)
    {
        _projMatrix = glm::perspective(_fovy, _aspectRatio, _zNear, _zFar);
    }
    else
    {
        if (_aspectRatio>1)
        {
            // w > h
            _projMatrix = glm::ortho(-1.0f, 1.0f, -_aspectRatio, _aspectRatio, _zNear, _zFar);
        }
        else
        {
            _projMatrix = glm::ortho(-_aspectRatio, _aspectRatio, -1.0f, 1.0f, _zNear, _zFar);
        }
    }
    
    _needUpdateFrustum = false;
}
