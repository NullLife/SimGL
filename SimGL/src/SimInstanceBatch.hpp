//
//  SimInstanceBatch.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimInstanceBatch_hpp
#define SimInstanceBatch_hpp

#include "SimCommon.hpp"
#include "SimRenderable.hpp"
#include "SimMovableObject.hpp"
#include "SimRenderable.hpp"
#include "SimTransform.hpp"

class InstancedModel;
class BatchInstance;

class InstanceBatch : public Renderable, public MovableObject
{
public:
    typedef Vector<InstancedModel*> InstanceModelList;
    typedef Vector<BatchInstance*> BatchInstanceList;

    InstanceBatch(const String &name, const String& meshName, const String& mtlName);
    virtual ~InstanceBatch();
    
    void setNumInstancePerBatch(size_t num) { _numInstancesPerBatch = num; }
    const size_t getNumInstancePerBatch() const { return _numInstancesPerBatch; }
    
    const MeshPtr& getMesh() { return _sharedMesh; }
    
    void createAllInstances();
    const InstanceModelList& getInstances() const { return _instances; }
    
    void build();

    const MaterialPtr& getMaterial() { return _sharedMtl; }
    
    virtual void getRenderOperation(RenderOperation& op) { op = _op; }
    
    virtual const Mat4& getWorldTransforms() { return _parent->getTransform()->getModelMatrix(); }
    
    virtual void updateRenderQueue(RenderQueue* queue);
    
    void _buildFrom(SubMesh* subMesh, BatchInstance* batch);
    
private:
    InstanceModelList _instances;
    BatchInstanceList _batchInstances;
    
    size_t _numInstancesPerBatch;
    
    RenderOperation _op;
    
    String _meshName;
    MeshPtr _sharedMesh;
    
    String _mtlName;
    MaterialPtr _sharedMtl;
    
    bool _isBuilded;
};

class BatchInstance : public Renderable
{
public:
    BatchInstance(InstanceBatch* parent, int index);
    ~BatchInstance();
    
    void setVertexData(VertexData* verData) { _vertexData = verData; }
    const VertexData* getVertexData() { return _vertexData; }
    
    void setIndexData(IndexData* idxData) { _indexData = idxData; }
    const IndexData* getIndexData() { return _indexData; }
    
    const MaterialPtr& getMaterial() { return _parent->getMaterial(); }
    
    virtual const Mat4& getWorldTransforms() { return _parent->getWorldTransforms(); }
    
    virtual void getRenderOperation(RenderOperation& op);

private:
    InstanceBatch* _parent;
    int _index;
    VertexData* _vertexData;
    IndexData* _indexData;
};

#endif /* SimInstanceBatch_hpp */
