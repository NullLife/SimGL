//
//  SimParticleEmitter.hpp
//  SimGL
//
//  Created by yangbin on 2017/11/27.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParticleEmitter_hpp
#define SimParticleEmitter_hpp

#include "SimTransform.hpp"

class ParticleEmitter
{
public:
    ParticleEmitter(const String& name);
    ~ParticleEmitter();
    
    void setEmissionRate(const unsigned int numOfParticles) { _emissionRate = numOfParticles; }
    const unsigned int getEmissionRate() const { return _emissionRate; }
    
    void setPosition(const Vec3& pos) { _position = pos; }
    const Vec3& getPosition() const { return _position; }
    
    void setDirection(const Vec3& dir) { _direction = dir; }
    const Vec3& getDirection() const { return _direction; }
    
    void setMinMaxLifetime(const float min, const float max) { _minLifetime = min; _maxLifetime = max; }
    
private:
    String _name;
    
    /// Emit particles in a second.
    unsigned int _emissionRate;
    
    float _minLifetime;
    float _maxLifetime;
    
    Vec3 _position;
    Vec3 _direction;
};

#endif /* SimParticleEmitter_hpp */
