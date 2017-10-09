//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_TRANSFORM_HPP
#define SIMGL_TRANSFORM_HPP

#include "SimCommon.hpp"

class Transform
{
public:
    Transform();
    ~Transform();

    // Set position.
    void setPosition(const Vec3& position);
    const Vec3& getPosition();
    
    // Translation.
    void translate(float dx, float dy, float dz);
    
    // Scale.
    void scale(const Vec3& scaleF);
    
    // Rotate.
    void pitch(float rad);
    void yaw(float rad);
    void roll(float rad);
    
    void rotate (Quat quat);
    void rotate(Vec3 axis, float rad);
    
    const Mat4& getModelMatrix();
    
private:
    void update();
    
private:
    Mat4   _modelMatrix;
    Vec3   _pos;
    
    Quat  _orientation;
    float _pitch;
    float _yaw;
    float _roll;
    
    Vec3 _scale;
    Vec3 _trans;
    
    // Flag for update matrix.
    bool _needUpdateMatrix;
};


#endif //SIMGL_TRANSFORM_HPP
