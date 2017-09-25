//
//  SimFrustum.hpp
//  SimGL
//
//  Created by yangbin on 2017/9/4.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimFrustum_hpp
#define SimFrustum_hpp

#include "SimCommon.hpp"

enum ProjectionType
{
    PT_ORTHOGRAPHIC,
    PT_PERSPECTIVE
};

/** Worldspace clipping planes.
 */
enum FrustumPlane
{
    FRUSTUM_PLANE_NEAR   = 0,
    FRUSTUM_PLANE_FAR    = 1,
    FRUSTUM_PLANE_LEFT   = 2,
    FRUSTUM_PLANE_RIGHT  = 3,
    FRUSTUM_PLANE_TOP    = 4,
    FRUSTUM_PLANE_BOTTOM = 5
};

class Frustum
{
public:
    Frustum();
    virtual ~Frustum();
    
    virtual void setProjectionType(ProjectionType type);
    virtual ProjectionType getProjectionType();
    
    // Perspective
    virtual void fastInitFrustum(float fovy, float aspectRatio, float nearDist, float farDist);
    
    // Angle
    virtual void setFovy(float fovy);
    virtual float getFovy();
    
    virtual void setAspectRatio(float aspectRatio);
    virtual float getAspectRatio();
    
    virtual void setNearClipDistance(float nearDist);
    virtual float getNearClipDistance();
    
    virtual void setFarClipDistance(float farDist);
    virtual float getFarClipDistance();
    
    // Orthograhic
    virtual void setOrthWindow(float w, float h);
    virtual void setFrustumExtends(float left, float right, float bottom, float top);
    
    virtual const Mat4& getProjectiveMatrix();
    
    virtual void updateFrustum();
    
private:
    void _updateFrustumImp();
    
protected:
    ProjectionType _projType;
    
    float _fovy;
    float _aspectRatio;
    float _zNear;
    float _zFar;
    
    float _widowWidth;
    float _widowHeight;
    
    float _left;
    float _top;
    float _right;
    float _bottom;
    
    Mat4 _projMatrix;
    
    bool _needUpdateFrustum;
};

#endif /* SimFrustum_hpp */
