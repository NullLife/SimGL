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
    
    Particle(ParticleSystem* owner, const Vec3& pos, const Vec3& vel, const float birth, const float lifetime);
    
    void setParticleSystem(ParticleSystem* owner) { _owner = owner; }
    ParticleSystem* getParticleSystem() { return _owner; }
    
    void setPosition(const Vec3& pos) { _pos = pos; }
    const Vec3& getPosition() const { return _pos; }
    
    void setVelocity(const Vec3& vel) { _vel = vel; }
    const Vec3& getVelocity() const { return _vel; }
    
    void setSize(const float size) { _size = size; }
    const float getSize() const { return _size; }

    void setBirth(const float birth) { _birth = birth; }
    const float getBirth() const { return _birth; }
    
    void setLifetime(float lifetime) { _lifetime = lifetime; }
    const float getLifetime() const { return _lifetime; }
    
    virtual const MaterialPtr& getMaterial();
    
    virtual void getRenderOperation(RenderOperation& op);
    
    virtual const Mat4& getWorldTransforms();
    
private:
    ParticleSystem* _owner;
    
    Vec3 _pos;
    Vec3 _vel;
 
    float _size;
    
    float _birth;
    float _lifetime;
};

#endif /* SimParticle_hpp */
