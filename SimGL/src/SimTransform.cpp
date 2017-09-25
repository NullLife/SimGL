//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimTransform.hpp"

Transform::Transform() :
        mNeedUpdate(true),
        mPos(Vec3(0.0f)),
        mDistance(0.0f),
        mScaleX(1.0f), mScaleY(1.0f), mScaleZ(1.0f),
        mRotateX(0.0f), mRotateY(0.0f), mRotateZ(0.0f),
        mMatrix(Mat4(1.0f)) {

}

Transform::~Transform() {
    LogManager::getSingleton().debug("delete Transform");
}

void Transform::moveTo(const Vec3 &targetPos) {
    mDistance = targetPos - mPos;
    mPos = targetPos;
    
    // translate
    mMatrix = glm::translate(mMatrix, mDistance);
    mDistance = Vec3(0.0f);
}

void Transform::move(const Vec3& distance) {
    mDistance = distance;
    mPos += distance;
    mNeedUpdate = true;
    
    // translate
    mMatrix = glm::translate(mMatrix, mDistance);
    mDistance = Vec3(0.0f);
}

void Transform::rotateX(const float angle) {
    mRotateX = angle;
    mMatrix = glm::rotate(mMatrix, mRotateX, X_AXIS);
    mNeedUpdate = true;
}

void Transform::rotateY(const float angle) {
    mRotateY = angle;
    mMatrix = glm::rotate(mMatrix, mRotateY, Y_AXIS);
    mNeedUpdate = true;
}

void Transform::rotateZ(const float angle) {
    mRotateZ = angle;
    mMatrix = glm::rotate(mMatrix, mRotateZ, Z_AXIS);
    mNeedUpdate = true;
}

void Transform::scaleX(const float factor) {
    mScaleX = factor;
    mNeedUpdate = true;
}

void Transform::scaleY(const float factor) {
    mScaleY = factor;
    mNeedUpdate = true;
}

void Transform::scaleZ(const float factor) {
    mScaleZ = factor;
    mNeedUpdate = true;
}

void Transform::setPosition(const Vec3& pos) {
    mDistance = pos - mPos;
    mPos = pos;
    mNeedUpdate = true;
}

Vec3 &Transform::getPosition() {
    updateMatrix();
    return mPos;
}

Mat4 &Transform::getMatrix() {
    updateMatrix();
    return mMatrix;
}

void Transform::updateMatrix() {
    if (!mNeedUpdate) {
        return;
    }
    
    // scale
    Vec3 scale(mScaleX, mScaleY, mScaleZ);
    mMatrix = glm::scale(mMatrix, scale);

    mScaleX = 1.0f;
    mScaleY = 1.0f;
    mScaleZ = 1.0f;

    mNeedUpdate = false;
}
