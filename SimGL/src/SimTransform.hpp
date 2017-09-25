//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_TRANSFORM_HPP
#define SIMGL_TRANSFORM_HPP

#include "SimCommon.hpp"

class Transform {
public:
    Transform();
    ~Transform();

private:
    Vec3 mPos;
    Mat4 mMatrix;

    float mScaleX;
    float mScaleY;
    float mScaleZ;

    float mRotateX;
    float mRotateY;
    float mRotateZ;

    Vec3 mDistance;   // Distance for translation

    bool mNeedUpdate;

public:
    void moveTo(const Vec3& targetPos);
    void move(const Vec3& distance);

    void rotateX(const float angle);
    void rotateY(const float angle);
    void rotateZ(const float angle);

    void scaleX(const float factor);
    void scaleY(const float factor);
    void scaleZ(const float factor);

    void setPosition(const Vec3& pos);
    Vec3& getPosition();

    Mat4& getMatrix();

private:
    void updateMatrix();
};


#endif //SIMGL_TRANSFORM_HPP
