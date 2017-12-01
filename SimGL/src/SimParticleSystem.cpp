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

#include "SimTransform.hpp"

#include <stdlib.h>

#define SEED(time) srand((int)time);
#define RANDOM(x) (rand()%x)

ParticleSystem::ParticleSystem(const String& name, size_t poolSize) :
    MovableObject(name),
    Renderable(),
    _polyMode(PolygonMode::PM_FILL),
    _drawType(DrawType::DT_POINTS),
    _poolSize(poolSize),
    _drawBuffer(0)
{
    _init();
    setRenderer(new ParticleSystemRenderer());
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
    setRenderQueueGroup(RQT_PARTICLES);
    
    // Initialise the system particle pool.
    _particlePool.resize(_poolSize);
    _particlePool.reserve(_poolSize);
    
    for (size_t i=0; i<_poolSize; ++i)
    {
        Particle* p = new Particle();
        
        p->setBirth(glfwGetTime() + RANDOM(100)/100.0f * 10 + 1);
        float life = RANDOM(100)/100.0f * 4 + 2;
        p->setLifetime(life);
        
        p->setPosition(Vec3(RANDOM(100)/100.0f*10.0f - 5.0f, 0.5f, RANDOM(100)/100.0f*10.0f - 5.0f));
        
        p->setParticleSystem(this);
        
        // Fill system pool.
        _particlePool[i] = p;
        
        // Fill free pool.
        _freePool.push_back(p);
    }
    
    // Create share buffer.
    _createBuffer();
}

Particle* ParticleSystem::createParticle(const Vec3& position, const Vec3& velocity, const float lifetime)
{
    // Take a particle out from free pool.
    Particle* p = _freePool.front();
    _activePool.splice(_activePool.end(), _freePool, _freePool.begin());
    
    p->setParticleSystem(this);
    p->setPosition(position);
    p->setLifetime(lifetime),
    p->setVelocity(velocity);
    
    return p;
}

void ParticleSystem::_createBuffer()
{
    glGenVertexArrays(2, _tfVaos);
    glGenVertexArrays(2, _renderVaos);
    glGenTransformFeedbacks(2, _tfos);
    
    //=============== Vertex data ===============//
    // Vertex [position, uv].
    VertexDataDeclare* vddVertexPos = new VertexDataDeclare(VEL_ALIGN);
    vddVertexPos->addElement(VES_POSITION, VET_FLOAT3, 4);
    vddVertexPos->addElement(VES_TEXCOORD, VET_FLOAT3, 4);
    Vector<Vec3> verPosData;
    verPosData.push_back(Vec3(-0.5, -0.5, 0));
    verPosData.push_back(Vec3(0.5, -0.5, 0));
    verPosData.push_back(Vec3(0.5, 0.5, 0));
    verPosData.push_back(Vec3(-0.5, 0.5, 0));
    
    verPosData.push_back(Vec3(0, 1, 0));
    verPosData.push_back(Vec3(1, 1, 0));
    verPosData.push_back(Vec3(1, 0, 0));
    verPosData.push_back(Vec3(0, 0, 0));
    
    _vertexBuf = new VertexData(vddVertexPos);
    _vertexBuf->setNumberVertices(4);
    _vertexBuf->createBuffer(vddVertexPos->getStride(), HardwareBuffer::Usage::HBU_STATIC);
    _vertexBuf->getBuffer()->writeData(&verPosData[0]);

    // Vertex index.
    Vector<unsigned int> verIndexData;
    verIndexData.push_back(0);verIndexData.push_back(1);verIndexData.push_back(2);
    verIndexData.push_back(0);verIndexData.push_back(2);verIndexData.push_back(3);
    _vertexIndexBuf = new IndexData();
    _vertexIndexBuf->createBuffer(HardwareIndexBuffer::IndexType::IT_UInt, 6, HardwareBuffer::Usage::HBU_STATIC);
    _vertexIndexBuf->getBuffer()->writeData(&verIndexData[0]);

    //=============== Instance data ================//
    // Instance position.
    VertexDataDeclare* vddInsPos = new VertexDataDeclare(VEL_ALIGN);
    vddInsPos->addElement(VES_INSTANCE_POSITION, VET_FLOAT3);
    _insPosBufs.resize(2);
    _insPosBufs.reserve(2);
    // Instance velocity.
    VertexDataDeclare* vddInsVelocity = new VertexDataDeclare(VEL_ALIGN);
    vddInsVelocity->addElement(VES_INSTANCE_VELOCITY, VET_FLOAT3);
    _insVelBufs.resize(2);
    _insVelBufs.reserve(2);
    // Instance size.
    VertexDataDeclare* vddInsSize = new VertexDataDeclare(VEL_ALIGN);
    vddInsSize->addElement(VES_INSTANCE_SIZE, VET_FLOAT1);
    _insSizeBufs.resize(2);
    _insSizeBufs.reserve(2);
    // Instance birth.
    VertexDataDeclare* vddInsBirth = new VertexDataDeclare(VEL_ALIGN);
    vddInsBirth->addElement(VES_INSTANCE_BIRTH, VET_FLOAT1);
    _insBirthBufs.resize(2);
    _insBirthBufs.reserve(2);
    // Instance lifetime.
    VertexDataDeclare* vddInsLifetime = new VertexDataDeclare(VEL_ALIGN);
    vddInsLifetime->addElement(VES_INSTANCE_LIFETIME, VET_FLOAT1);
    _insLifetimeBufs.resize(2);
    _insLifetimeBufs.reserve(2);
    // Instance life rate.
    VertexDataDeclare* vddInsLifeRate = new VertexDataDeclare(VEL_ALIGN);
    vddInsLifeRate->addElement(VES_INSTANCE_LIFERATE, VET_FLOAT1);
    _insLifeRateBufs.resize(2);
    _insLifeRateBufs.reserve(2);

    // Only alloc size.
    for (int i=0; i<2; ++i)
    {
        _insPosBufs[i] = new VertexData(vddInsPos->clone());
        _insPosBufs[i]->setNumberVertices(_poolSize);
        _insPosBufs[i]->createBuffer(_poolSize * sizeof(Vec3), HardwareBuffer::Usage::HBU_DYNAMIC);
        
        _insVelBufs[i] = new VertexData(vddInsVelocity->clone());
        _insVelBufs[i]->setNumberVertices(_poolSize);
        _insVelBufs[i]->createBuffer(_poolSize * sizeof(Vec3), HardwareBuffer::Usage::HBU_DYNAMIC);
        
        _insSizeBufs[i] = new VertexData(vddInsSize->clone());
        _insSizeBufs[i]->setNumberVertices(_poolSize);
        _insSizeBufs[i]->createBuffer(_poolSize * sizeof(float), HardwareBuffer::Usage::HBU_STATIC);
        
        _insBirthBufs[i] = new VertexData(vddInsBirth->clone());
        _insBirthBufs[i]->setNumberVertices(_poolSize);
        _insBirthBufs[i]->createBuffer(_poolSize * sizeof(float), HardwareBuffer::Usage::HBU_DYNAMIC);
        
        _insLifetimeBufs[i] = new VertexData(vddInsLifetime->clone());
        _insLifetimeBufs[i]->setNumberVertices(_poolSize);
        _insLifetimeBufs[i]->createBuffer(_poolSize * sizeof(float), HardwareBuffer::Usage::HBU_STATIC);
        
        _insLifeRateBufs[i] = new VertexData(vddInsLifeRate->clone());
        _insLifeRateBufs[i]->setNumberVertices(_poolSize);
        _insLifeRateBufs[i]->createBuffer(_poolSize * sizeof(float), HardwareBuffer::Usage::HBU_DYNAMIC);
    }
    
    delete vddInsPos;
    delete vddInsVelocity;
    delete vddInsSize;
    delete vddInsBirth;
    delete vddInsLifetime;
    delete vddInsLifeRate;
    
    Vector<Vec3> insPosData;
    Vector<Vec3> insVelocityData;
    Vector<float> insSizeData;
    Vector<float> insBirthData;
    Vector<float> insLifetimeData;
    Vector<float> insLifeRateData;
    
    for (size_t i=0; i<_poolSize; ++i)
    {
        Particle* p = _particlePool[i];
        
        insPosData.push_back(p->getPosition());
        insVelocityData.push_back(p->getVelocity());
        insSizeData.push_back(p->getSize());
        insBirthData.push_back(p->getBirth());
        insLifetimeData.push_back(p->getLifetime());
        insLifeRateData.push_back(0.0f);
    }
    
    // First rendering
    _insPosBufs[1]->getBuffer()->writeData(&insPosData[0]);
    _insVelBufs[1]->getBuffer()->writeData(&insVelocityData[0]);
    _insSizeBufs[0]->getBuffer()->writeData(&insSizeData[0]);
//    _insSizeBufs[1]->getBuffer()->writeData(&insSizeData[0]);
    _insBirthBufs[1]->getBuffer()->writeData(&insBirthData[0]);
    _insLifetimeBufs[0]->getBuffer()->writeData(&insLifetimeData[0]);
//    _insLifetimeBufs[1]->getBuffer()->writeData(&insLifetimeData[0]);
    _insLifeRateBufs[1]->getBuffer()->writeData(&insLifeRateData[0]);
    
    // Binding transform feedback VAOs.
    for (int i=0; i<2; ++i)
    {
        glBindVertexArray(_tfVaos[i]);
        
        _insPosBufs[i]->unbind();
        _insPosBufs[i]->bindAttr(2, true);
        
        _insVelBufs[i]->unbind();
        _insVelBufs[i]->bindAttr(3, true);
        
        _insSizeBufs[0]->unbind();
        _insSizeBufs[0]->bindAttr(4, true);
        
        _insBirthBufs[i]->unbind();
        _insBirthBufs[i]->bindAttr(5, true);
        
        _insLifetimeBufs[0]->unbind();
        _insLifetimeBufs[0]->bindAttr(6, true);
        
        _insLifeRateBufs[i]->unbind();
        _insLifeRateBufs[i]->bindAttr(7, true);
        
        glBindVertexArray(0);
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _tfos[i]);
        
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _insPosBufs[i]->getBuffer()->getBufferId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, _insVelBufs[i]->getBuffer()->getBufferId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, _insBirthBufs[i]->getBuffer()->getBufferId());
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 3, _insLifeRateBufs[i]->getBuffer()->getBufferId());
        
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    }

    // Binding rendering VAOs.
    for (int i=0; i<2; ++i)
    {
        glBindVertexArray(_renderVaos[i]);
        
        // Note: Must bind it and the states is accumulative in VAO.
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexIndexBuf->getBuffer()->getBufferId());
    
        _vertexBuf->unbind();
        _vertexBuf->bind(false);
        
        _insPosBufs[i]->unbind();
        _insPosBufs[i]->bindAttr(2, true);
        
        _insVelBufs[i]->unbind();
        _insVelBufs[i]->bindAttr(3, true);

        _insSizeBufs[0]->unbind();
        _insSizeBufs[0]->bindAttr(4, true);

        _insBirthBufs[i]->unbind();
        _insBirthBufs[i]->bindAttr(5, true);

        _insLifetimeBufs[i]->unbind();
        _insLifetimeBufs[i]->bindAttr(6, true);
        
        _insLifeRateBufs[i]->unbind();
        _insLifeRateBufs[i]->bindAttr(7, true);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    // Release data.
    verPosData.clear();
    verPosData.shrink_to_fit();
    
    verIndexData.clear();
    verIndexData.shrink_to_fit();
    
    insPosData.clear();
    insPosData.shrink_to_fit();
    
    insVelocityData.clear();
    insVelocityData.shrink_to_fit();

    insSizeData.clear();
    insSizeData.shrink_to_fit();

    insBirthData.clear();
    insBirthData.shrink_to_fit();
    
    insLifetimeData.clear();
    insLifetimeData.shrink_to_fit();
    
    insLifeRateData.clear();
    insLifeRateData.shrink_to_fit();
}

void ParticleSystem::_destroyBuffer()
{
    delete _vertexBuf;
    _vertexBuf = nullptr;
    
    delete _vertexIndexBuf;
    _vertexIndexBuf = nullptr;
    
    delete _insPosBufs[0];
    delete _insPosBufs[1];
    _insPosBufs.clear();
    _insPosBufs.shrink_to_fit();
    
    delete _insVelBufs[0];
    delete _insVelBufs[1];
    _insVelBufs.clear();
    _insVelBufs.shrink_to_fit();
    
    delete _insSizeBufs[0];
    delete _insSizeBufs[1];
    _insSizeBufs.clear();
    _insSizeBufs.shrink_to_fit();
    
    delete _insBirthBufs[0];
    delete _insBirthBufs[1];
    _insBirthBufs.clear();
    _insBirthBufs.shrink_to_fit();
    
    delete _insLifetimeBufs[0];
    delete _insLifetimeBufs[1];
    _insLifetimeBufs.clear();
    _insLifetimeBufs.shrink_to_fit();
    
    delete _insLifeRateBufs[0];
    delete _insLifeRateBufs[1];
    _insLifeRateBufs.clear();
    _insLifeRateBufs.shrink_to_fit();

    glDeleteTransformFeedbacks(2, _tfos);
    glDeleteVertexArrays(2, _tfVaos);
    glDeleteVertexArrays(2, _renderVaos);
}

void ParticleSystem::clear()
{
    _freePool.splice(_freePool.end(), _activePool);
}

ParticleSystemRenderer* ParticleSystem::getParticleSystemRenderer()
{
    return (ParticleSystemRenderer *) _renderer;
}

void ParticleSystem::setMaterial(const String& mtlName)
{
    getParticleSystemRenderer()->setMaterial(mtlName);
}

const MaterialPtr& ParticleSystem::getMaterial()
{
    return getParticleSystemRenderer()->getMaterial();
}

void ParticleSystem::getRenderOperation(RenderOperation& op)
{
    op._obj = this;
    op._drawType = DrawType::DT_TRIANGLES;
    op._hasInstancesData = true;
    op._useIndex = true;
    
    op._vertexData = _vertexBuf;
    op._indexData = _vertexIndexBuf;
    
    op._start = 0;
    op._count = 6;
    
    op._numOfInstances = _poolSize;
}

const Mat4& ParticleSystem::getWorldTransforms()
{
    return getParentNode()->getTransform()->getModelMatrix();
}

void ParticleSystem::updateRenderQueue(RenderQueue* queue)
{
    // Only one renderable.
    queue->addRenderable(this);
}

void ParticleSystem::_updateParticles(GLRenderSystem* system, RenderOperation& op, Pass* pass)
{
    ParticleSystemRenderer* render = getParticleSystemRenderer();
    
    // Disable rendering.
    render->offRasterizer();
    
    glBindVertexArray(_tfVaos[1-_drawBuffer]);
    
    // Bind feedback object for the buffer to be drawn next.
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _tfos[_drawBuffer]);
    
    render->startTransfromFeedback(DT_POINTS);
    
    glDrawArraysInstanced(DT_POINTS, 0, 1, (GLsizei) op._numOfInstances);
    
    render->stopTransfromFeedback();

//    Vector<float> feedbackData((GLsizei) _insPosBufs[_drawBuffer]->getBuffer()->getSize()/sizeof(float));
//    glBindBuffer(GL_ARRAY_BUFFER, _insPosBufs[_drawBuffer]->getBuffer()->getBufferId());
//    glGetBufferSubData(GL_ARRAY_BUFFER,
//                       0,
//                       (GLsizei) _insPosBufs[_drawBuffer]->getBuffer()->getSize(),
//                       &feedbackData[0]);
//
//    LogManager::getSingleton().debug("TransfromFeedback", " ======== feedbackData =========");
//    for (int i=0; i<feedbackData.size(); ++i)
//        LogManager::getSingleton().debug("TransfromFeedback", "feedbackData: " + StringUtils::toString(feedbackData[i]));
    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    
    glBindVertexArray(0);
    
    // Enable rendering.
    render->onRasterizer();
}

void ParticleSystem::_renderParticles(GLRenderSystem* system, RenderOperation& op, Pass* pass)
{
    glBindVertexArray(_renderVaos[_drawBuffer]);
    
    glDrawElementsInstanced(op._drawType,
                            (GLsizei) op._count,
                            op._indexData->getBuffer()->getIndexType(),
                            0,
                            (GLsizei) op._numOfInstances);
    
    glBindVertexArray(0);
    
    // Swap buffers.
    _drawBuffer = 1 - _drawBuffer;
}

