//
//  SimParticleSystem.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParticleSystem.hpp"

#include "SimParticle.hpp"
#include "SimRenderQueue.hpp"
#include "SimParticleSystemRenderer.hpp"

#include "SimVertexArrayManager.hpp"

ParticleSystem::ParticleSystem(const String& name, size_t poolSize) :
    MovableObject(name),
    _renderer(new ParticleSystemRenderer()),
    _polyMode(PolygonMode::PM_FILL),
    _drawType(DrawType::DT_POINTS),
    _poolSize(poolSize)
{
    _init();
}

ParticleSystem::~ParticleSystem()
{
    ParticlePool::iterator iter = _particlePool.begin();
    ParticlePool::const_iterator endIter = _particlePool.end();
    while (iter != endIter)
    {
        delete *iter;
        ++iter;
    }
    _particlePool.clear();
    _particlePool.shrink_to_fit();
    
    _destroyBuffer();
}

void ParticleSystem::_init()
{
    _particlePool.resize(_poolSize);
    for (size_t i=0; i<_poolSize; ++i)
    {
        Particle* p = new Particle();
        p->setParticleSystem(this);
        _particlePool[i] = p;
        _freePool.push_back(p);
    }
    
    _createBuffer();
}

Particle* ParticleSystem::createParticle(const Vec3 &pos)
{
    Particle* p = _freePool.front();
    _activePool.splice(_activePool.end(), _freePool, _freePool.begin());
    
    p->setParticleSystem(this);
    p->setPosition(pos);
    p->setLifetime(0.0f),
    p->setColor(Vec4(1.0f));
    p->setVelocity(Vec3(0, 1.0f, 0));
    
    return p;
}

void ParticleSystem::_createBuffer()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    VertexDataDeclare* vdd = new VertexDataDeclare();
    vdd->addElement(new VertexElement(VES_POSITION, VET_FLOAT3));     // position.
    vdd->addElement(new VertexElement(VES_DIFFUSECOLOR, VET_FLOAT4)); // color.
    vdd->addElement(new VertexElement(VES_VELOCITY, VET_FLOAT3));     // velocity.
    vdd->addElement(new VertexElement(VES_LIFETIME, VET_FLOAT1));     // lifetime.
    
    _sharedVertexData = new VertexData(vdd);
    
    std::vector<Vec3> data;
    data.push_back(Vec3(0.0f));
    data.push_back(Vec4(1.0f, 0, 0, 1.0f));
    data.push_back(Vec3(0, 1.0f, 0));
    data.push_back(Vec3(0.0f));
    
    HardwareVertexBuffer* buffer = _sharedVertexData->createBuffer(vdd->getStride(), 1, HardwareBuffer::Usage::HBU_STATIC);
    buffer->writeData(&data[0]);
    
    glBindVertexArray(0);
    
    // Cache vao object.
    VertexArrayManager::getSingleton().cacheVao(buffer->getBufferId(), vao);
}

void ParticleSystem::_destroyBuffer()
{
    if (_sharedVertexData)
    {
        VertexArrayManager::getSingleton().deleteVao(_sharedVertexData->getBuffer()->getBufferId());
        delete _sharedVertexData;
        _sharedVertexData = nullptr;
    }
}

void ParticleSystem::clear()
{
    _freePool.splice(_freePool.end(), _activePool);
}

void ParticleSystem::setMaterial(const String& mtlName)
{
    _renderer->setMaterial(mtlName);
}

const MaterialPtr& ParticleSystem::getMaterial()
{
    return _renderer->getMaterial();
}

void ParticleSystem::setRenderQueueGroup(const int queueID)
{
    MovableObject::setRenderQueueGroup(queueID);
}

void ParticleSystem::updateRenderQueue(RenderQueue* queue)
{
    for (Particle* p : _particlePool)
    {
        queue->addRenderable(p);
    }
}
