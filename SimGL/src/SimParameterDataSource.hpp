//
//  FMKParameterDataSource.hpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/31.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParameterDataSource_hpp
#define SimParameterDataSource_hpp

#include "SimLight.hpp"

class Camera;
class Pass;

class ParameterDataSource
{
private:
    mutable Mat4 _modelMatrix;
    mutable Mat4 _viewMatrix;
    mutable Mat4 _projectionMatrix;
    
    mutable Mat4 _mvMatrix;
    mutable Mat4 _mvpMatrix;
    
    mutable Vec4 _ambientLightColor;
    
    mutable bool _modelMatrixDirty;
    mutable bool _viewMatrixDirty;
    mutable bool _projectionMatrixDirty;
    mutable bool _mvMatrixDirty;
    mutable bool _mvpMatrixDirty;
    
    mutable float _time;
    
    Camera* _currCamera;
    LightList _lights;
    Pass* _currPass;
    
public:
    ParameterDataSource();
    ~ParameterDataSource();
    
    void setModelMatrix(const Mat4& m);
    
    void setAmbientLightColor(const Vec4& color);
    
    void setCurrentCamera(Camera* cam);
    
    void setLights(const LightList& lights);
    
    void setCurrentPass(Pass* pass);
    
    void setTime(float time);
    
    // ---------------------------------------//
    
    const Mat4& getModelMatrix();
    const Mat4& getViewMatrix();
    const Mat4& getProjectionMatrix();
    
    const Mat4& getModelViewMatrix();
    const Mat4& getModelViewProjectionMatrix();

    const Vec3& getCameraPosition();
    
    const Vec4& getAmbientLightColor();
    
    // ============ Light ============= //
    const int getLightCount();
    const int getLightType(int index);
    const Vec3& getLightPosition(int index);
    const Vec3& getLightDirection(int index);
    
    const Vec4& getLightDiffuseColor(int index);
    const Vec4& getLightSpecularColor(int index);
    
    const float getSpotLightCutoff(int index);
    
    const float getLightIntensity(int index);
    const Vec4& getLightAttenuation(int index);
    
    // ============ Material ============= //
    const Vec4& getSurfaceAmbient();
    const Vec4& getSurfaceDiffuse();
    const Vec4& getSurfaceSpecular();
    const Vec4& getSurfaceEmission();
    const float getSurfaceShininess();
    
    // ============= Other ============== //
    const float getTime();
};

#endif /* SimParameterDataSource_hpp */
