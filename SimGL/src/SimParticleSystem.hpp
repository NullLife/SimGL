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
#include "SimRenderable.hpp"
#include "SimRenderOperation.hpp"
#include "SimVertexIndexData.hpp"
#include "SimRenderer.hpp"

class Particle;
class ParticleSystemRenderer;

/**
 * In this particle system, we will use the instancing render technique,
 * that is say, all particles will share a vertice data.
 * And we will use transform feedback to update a particle properties.
 *
 * There are two pass. one is for update particle using transform feedback (no rasterization),
 * another one for rendering (rasterization).
 * --------------------------------------------------------------
 * | Input Buffer A-----> Vertex shader -----> Out Buffer B;    |
 * | Input Buffer B-----> Vertex shader -----> Fragment shader. |
 * --------------------------------------------------------------
 */
class ParticleSystem : public MovableObject, public Renderable
{
public:
    typedef List<Particle*> FreeParticlePool;
    typedef List<Particle*> ActiveParticlePool;
    typedef Vector<Particle*> ParticlePool;
    
    ParticleSystem(const String& name, size_t poolSize);
    ~ParticleSystem();
    
    const size_t getPoolSize() const { return _poolSize; }
    
    /// Create a particle.
    Particle* createParticle(const Vec3& position, const Vec3& velocity, const float lifetime);
    
    /// Clear active particles.
    void clear();
    
    void setPolygonMode(const PolygonMode mode) { _polyMode = mode; }
    const PolygonMode getPolygonMode() const { return _polyMode; }
    
    void setDrawType(const DrawType type) { _drawType = type; }
    const DrawType getDrawType() const { return _drawType; }
    
    void setMaterial(const String& mtlName);
    
    virtual void updateRenderQueue(RenderQueue* queue);
    
    // Renderable
    const MaterialPtr& getMaterial();
    
    void getRenderOperation(RenderOperation& op);
    
    const Mat4& getWorldTransforms();
    
    ParticleSystemRenderer* getParticleSystemRenderer();
    
    //-----------------------------//
    void _updateParticles(GLRenderSystem* system, RenderOperation& op, Pass* pass);
    void _renderParticles(GLRenderSystem* system, RenderOperation& op, Pass* pass);
    
private:
    /// Initialise the system.
    void _init();
    
    /// Create a shared vertex buffer.
    void _createBuffer();
    
    /// Destory the shared vertex buffer.
    void _destroyBuffer();
    
    //===========================================//
    
    /// The max number of particles.
    size_t _poolSize;
    
    /// The free pool for particle reusing.
    FreeParticlePool _freePool;
    
    /// Active particles.
    ActiveParticlePool _activePool;
    
    /// The system particle pool.
    ParticlePool _particlePool;
    
    PolygonMode _polyMode;
    DrawType _drawType;
    
    // Flag for swapping buffer.
    int _drawBuffer;
    
    VertexData* _vertexBuf;
    IndexData*  _vertexIndexBuf;
    
    /// Two buffer, one for update, another for render.
    Vector<VertexData*> _insPosBufs;
    Vector<VertexData*> _insVelBufs;
    Vector<VertexData*> _insSizeBufs;
    Vector<VertexData*> _insBirthBufs;
    Vector<VertexData*> _insLifetimeBufs;
    Vector<VertexData*> _insLifeRateBufs;
    
    /// Transform feedback objects.
    GLuint _tfos[2];
    
    GLuint _tfVaos[2];
    GLuint _renderVaos[2];
};

#endif /* SimParticleSystem_hpp */
