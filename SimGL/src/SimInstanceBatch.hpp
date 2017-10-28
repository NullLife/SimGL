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

class InstanceBatch : public Renderable, public MovableObject
{
public:
    typedef Vector<InstancedModel*> InstanceModelList;
public:
    InstanceBatch(const String &name, const String& meshName, const String& mtlName);
    virtual ~InstanceBatch();
    
    void setNumInstancePerBatch(size_t num) { _numInstancesPerBatch = num; }
    const size_t getNumInstancePerBatch() const { return _numInstancesPerBatch; }
    
    const MeshPtr& getMesh() { return _sharedMesh; }
    
    void createAllInstances();

    const MaterialPtr& getMaterial() { return _sharedMtl; }
    
    virtual void getRenderOperation(RenderOperation& op) { op = _op; }
    
    virtual const Mat4& getWorldTransforms() { return mParent->getTransform()->getModelMatrix(); }
    
    virtual void updateRenderQueue(RenderQueue* queue);
    
private:
    InstanceModelList _instances;
    
    size_t _numInstancesPerBatch;
    
    RenderOperation _op;
    
    MeshPtr _sharedMesh;
    MaterialPtr _sharedMtl;
};

#endif /* SimInstanceBatch_hpp */
