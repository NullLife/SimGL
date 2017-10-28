//
//  SimParticle.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParticle.hpp"

#include "SimTransform.hpp"

Particle::Particle() :
    Renderable(),
    _owner(nullptr),
    _pos(Vec3(0)),
    _vel(Vec3(0, 0.5f, 0)),
    _color(Vec4(1.0f)),
    _lifetime(1.5f)
{}

Particle::Particle(ParticleSystem* owner, const Vec3& pos, const Vec3& vel, const Vec4& color, const float lifetime) :
    Renderable(),
    _owner(owner),
    _pos(pos),
    _vel(vel),
    _color(color),
    _lifetime(lifetime)
{}

Particle::~Particle()
{}

const MaterialPtr& Particle::getMaterial()
{
    return _owner->getMaterial();
}

void Particle::getRenderOperation(RenderOperation& op)
{
    op._drawType = _owner->getDrawType();
    op._obj = this;
    
    op._vertexData = _owner->getSharedVertexData();
    op._indexData = nullptr;
    op._useIndex = false;
    
    op._start = 0;
    op._count = op._vertexData->getNumVertices();
    
    op._hasInstancesData = true;
    op._numOfInstances = _owner->getPoolSize();
}

const Mat4& Particle::getWorldTransforms()
{
    return _owner->getParentNode()->getTransform()->getModelMatrix();
}

