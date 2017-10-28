//
//  SimParticleSystem.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParticleSystem_hpp
#define SimParticleSystem_hpp

#include "SimCommon.hpp"
#include "SimMovableObject.hpp"
#include "SimRenderOperation.hpp"
#include "SimVertexIndexData.hpp"

class Particle;
class ParticleSystemRenderer;

class ParticleSystem : public MovableObject
{
public:
    typedef List<Particle*> FreeParticlePool;
    typedef List<Particle*> ActiveParticlePool;
    typedef Vector<Particle*> ParticlePool;
    
    ParticleSystem(const String& name, size_t poolSize);
    ~ParticleSystem();
    
    const size_t getPoolSize() const { return _poolSize; }
    
    Particle* createParticle(const Vec3& pos);
    
    void clear();
    
    void setPolygonMode(const PolygonMode mode) { _polyMode = mode; }
    const PolygonMode getPolygonMode() const { return _polyMode; }
    
    void setDrawType(const DrawType type) { _drawType = type; }
    const DrawType getDrawType() const { return _drawType; }
    
    VertexData* getSharedVertexData() { return _sharedVertexData; }
    
    //void setRenderer(ParticleSystemRenderer* renderer) { _renderer = renderer; }
    ParticleSystemRenderer* getRenderer() { return _renderer; }
    
    void setMaterial(const String& mtlName);
    const MaterialPtr& getMaterial();
    
    void setRenderQueueGroup(const int queueID);
    virtual void updateRenderQueue(RenderQueue* queue);
    
private:
    void _init();
    
    void _createBuffer();
    void _destroyBuffer();
    
    size_t _poolSize;
    
    FreeParticlePool _freePool;
    ActiveParticlePool _activePool;
    ParticlePool _particlePool;
    
    PolygonMode _polyMode;
    DrawType _drawType;
    
    VertexData* _sharedVertexData;

    ParticleSystemRenderer* _renderer;
};

#endif /* SimParticleSystem_hpp */
