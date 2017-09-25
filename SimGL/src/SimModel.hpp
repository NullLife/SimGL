//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MODEL_HPP
#define SIMGL_MODEL_HPP

#include "SimMeshManager.hpp"
#include "SimMovableObject.hpp"
#include "SimSubModel.hpp"

class Mesh;
class SceneNode;

class Model : public MovableObject {
public:
    typedef Vector<SubModel*> SubModelList;
public:
    Model(const String &name);
    Model(const MeshPtr& meshPtr);

    ~Model();

private:
    MeshPtr mMeshPtr;
    SubModelList mSubModelList;

public:
    const String& getName() { return mName; }
    
    const MeshPtr& getMesh();

    void setParent(SceneNode* node);
    
    void setMaterial(const MaterialPtr& mtlPtr);
    
    void setMaterial(const String& material);

    const SubModelList& getSubModels();
    
    void setRenderQueueGroup(const int queueID);
    
    void updateRenderQueue(RenderQueue* queue);

private:
    MeshPtr _loadMesh(const String& name);
    
    void _buildSubModelList(const MeshPtr& meshPtr);
};


#endif //SIMGL_MODEL_HPP
