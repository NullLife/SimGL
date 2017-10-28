//
//  SimInstanceBatch.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimInstanceBatch.hpp"

#include "SimMeshManager.hpp"
#include "SimMaterialManager.hpp"

#include "SimInstancedModel.hpp"

InstanceBatch::InstanceBatch(const String &name, const String& meshName, const String& mtlName) :
    Renderable(),
    MovableObject(name),
    _numInstancesPerBatch(1)
{
    _sharedMesh = MeshManager::getSingleton().getMesh(meshName);
    _sharedMtl = MaterialManager::getSingleton().get(mtlName);
}

InstanceBatch::~InstanceBatch()
{
}

void InstanceBatch::createAllInstances()
{
    _instances.reserve(_numInstancesPerBatch);
    for (unsigned int i=0; i<_numInstancesPerBatch; ++i)
    {
        InstancedModel* ins = new InstancedModel(this, i);
        _instances.push_back(ins);
    }
}

void InstanceBatch::updateRenderQueue(RenderQueue* queue)
{
    
}
