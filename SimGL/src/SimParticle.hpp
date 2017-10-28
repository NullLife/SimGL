//
//  SimParticle.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParticle_hpp
#define SimParticle_hpp

#include "SimParticleSystem.hpp"
#include "SimRenderable.hpp"

class Particle : public Renderable
{
public:
    friend class ParticleSystem;
    
    Particle();
    ~Particle();
    
    Particle(ParticleSystem* owner, const Vec3& pos, const Vec3& vel, const Vec4& color, const float lifetime);
    
    void setParticleSystem(ParticleSystem* owner) { _owner = owner; }
    ParticleSystem* getParticleSystem() { return _owner; }
    
    void setPosition(const Vec3& pos) { _pos = pos; }
    const Vec3& getPosition() const { return _pos; }
    
    void setVelocity(const Vec3& vel) { _vel = vel; }
    const Vec3& getVelocity() const { return _vel; }
    
    void setLifetime(float lifetime) { _lifetime = lifetime; }
    const float getLifetime() const { return _lifetime; }
    
    void setColor(const Vec4& color) { _color = color; }
    const Vec4& getColor() const { return _color; }
    
    virtual const MaterialPtr& getMaterial();
    
    virtual void getRenderOperation(RenderOperation& op);
    
    virtual const Mat4& getWorldTransforms();
    
private:
    ParticleSystem* _owner;
    
    Vec3 _pos;
    Vec3 _vel;
    Vec4 _color;
    
    float _lifetime;
};

#endif /* SimParticle_hpp */
