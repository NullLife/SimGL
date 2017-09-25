//
//  SimLight.hpp
//  SimGL
//
//  Created by yangbin on 2017/5/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include "SimCommon.hpp"

enum LightType {
    LT_POINT_LIGHT = 0,
    LT_SPOT_LIGHT,
    LT_DIRECTION_LIGHT
};

class Light {
public:
    
    Light(const LightType type);
    ~Light();
    
    // Get light type.
    const LightType getType() const;
    
    // Set intensity of light.
    void setIntensity(float intensity);
    const float getIntensity() const;
    
    // Set diffuse color.
    void setDiffuseColor(const Vec4& color);
    const Vec4& getDiffuseColor() const;
    
    void setSpecularColor(const Vec4& color);
    const Vec4& getSpecularColor() const;
    
    // Set direction for directional light.
    void setDirection(const Vec3& direction);
    const Vec3& getDirection() const;
    
    // Set position.
    void setPosition(const Vec3& pos);
    const Vec3& getPosition() const;
    
    // Set cutoff for spot light.
    void setCutOff(float rad);
    const float getCutOff() const;
    
    // Set attenuation of light.
    void setAttenuation(float constant, float linear, float quadratic, float range);
    const Vec4& getAttenuation() const;
    
    // Returns the absolute upper range of the light.
    const float getAttenuationRange() const;
    
    // Returns the constant factor in the attenuation formula.
    const float getAttenuationConstant() const;
    
    // Returns the linear factor in the attenuation formula.
    const float getAttenuationLinear() const;
    
    // Returns the quadric factor in the attenuation formula.
    const float getAttenuationQuadric() const;
    
    // Max numbers of light source is 3.
    static int MAX_LIGHT_NUM;
    
private:
    // Light type.
    LightType _type;
    
    // Light lux.
    float _intensity;
    
    // Diffuse color.
    Vec4 _diffuseColor;
    
    // Highlight(specular) color.
    Vec4 _specularColor;
    
    // Light position for point light and spot light.
    Vec3 _position;
    
    // light direction for the light type diectional light and spot light.
    Vec3 _direction;
    
    // Spot light.
    float _cutoff;
    
    // Vec4f
    Vec4 _atten;
};

typedef Vector<Light*> LightList;

#endif /* Light_hpp */
