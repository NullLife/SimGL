//
//  SimParticleEmitter.cpp
//  SimGL
//
//  Created by yangbin on 2017/11/27.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParticleEmitter.hpp"

ParticleEmitter::ParticleEmitter(const String& name) :
    _name(name),
    _emissionRate(10),
    _minLifetime(2.0f),
    _maxLifetime(6.0f),
    _position(0, 0, 0),
    _direction(Y_AXIS)
{
}

ParticleEmitter::~ParticleEmitter()
{
}
