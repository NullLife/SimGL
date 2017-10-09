 //
//  SimCamera.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include "SimFrustum.hpp"

class Camera : public Frustum
{
public:
    Camera(const std::string& name);
    ~Camera();
    
    const std::string& getName();
    
    // Set position of camera in world coordinate.
    void setPosition(const Vec3& pos);
    const Vec3& getPosition();
    
    // Set the target of camera.
    void setTarget(const Vec3& target);
    const Vec3 getTarget();
    
    // Get up vector of camera.
    const Vec3 getUp();
    
    // Get look direction of camera.
    const Vec3 getLook();
    
    // Set distance range for camera position to target.
    void setDistanceRange(float minDist, float maxDist);
    
    // Get view matrix.
    const Mat4& getViewMatrix();
    
    // Get orientation angles.
    float getPitch();
    float getYaw();
    float getRoll();
    
    // Rotate
    void pitch(float rad);
    void yaw(float rad);
    void roll(float rad);
    
    void rotate (Quat quat);
    void rotate(Vec3 axis, float rad);
    
    // Tranlation.
    void translate(float dx, float dy, float dz);
    
    // Scale
    void scale(float scale);
    
    std::string toString(const Vec3& val);
    
private:
    void _updateView();
    void _updateFrustum();
    
private:
    // Camer name.
    std::string _name;
    
    // Camera attributes.
    Vec3 _camPos;
    Vec3 _camTarget;
    Vec3 _camUp;
    Vec3 _camRight;
    Vec3 _camLook;
    
    // Limit for camera.
    float _minDist;
    float _maxDist;
    
    float _curDist;
    
    // Camera oritentation.
    Quat _orientation;
    
    float _scale;
    
    Mat4 _viewMatrix;
    
    // Orientation.
    float _pitch;
    float _yaw;
    float _roll;
    
    // Flag for update view matrix.
    bool _needUpdateView;
};

#endif /* Camera_hpp */
