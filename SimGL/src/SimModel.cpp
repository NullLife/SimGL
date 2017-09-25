//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimModel.hpp"
#include "SimSceneNode.hpp"
#include "SimMesh.hpp"
#include "SimMaterialManager.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"


Model::Model(const String &name) :
        MovableObject(name) {
    mMeshPtr = _loadMesh(name);
}

Model::Model(const MeshPtr& meshPtr) :
        MovableObject(meshPtr->getName()),
        mMeshPtr(meshPtr) {
    _buildSubModelList(meshPtr);
}

Model::~Model() {
    SubModelList::iterator i;
    for (i=mSubModelList.begin(); i!=mSubModelList.end(); ++i) {
        delete (*i);
    }
    mSubModelList.clear();
    mSubModelList.shrink_to_fit();
    
    LogManager::getSingleton().debug("Delete Model", "name: " + mName);
}

const MeshPtr& Model::getMesh() {
    return mMeshPtr;
}

void Model::setParent(SceneNode* node) {
    mParent = node;
}

void Model::setMaterial(const MaterialPtr& mtlPtr)
{
    for (SubModel* subModel : mSubModelList)
    {
        subModel->setMaterial(mtlPtr);
    }
}

void Model::setMaterial(const String& material)
{
    setMaterial(MaterialManager::getSingleton().get(material));
}

const Model::SubModelList &Model::getSubModels() {
    return mSubModelList;
}

void Model::setRenderQueueGroup(const int queueID) {
    MovableObject::setRenderQueueGroup(queueID);
    for (SubModel* sub : mSubModelList) {
        sub->setRenderQueueGroup(queueID);
    }
}

void Model::updateRenderQueue(RenderQueue* queue) {
    for (SubModel* sub : mSubModelList) {
        if (sub->isVisible()) {
            queue->addRenderable(sub);
        }
    }
}

MeshPtr Model::_loadMesh(const String& name) {
    MeshPtr mesh = MeshManager::getSingleton().getMesh(name);
    _buildSubModelList(mesh);
    return mesh;
}

void Model::_buildSubModelList(const MeshPtr& mesh) {
    if (!mesh) {
        LogManager::getSingleton().debug("Model::buildSubModelList","mesh is null");
    }
    const Mesh::SubMeshList& subMeshList = mesh->getSubMeshList();
    mSubModelList.resize(subMeshList.size());
    
    int index = 0;
    for (SubMesh* subMesh : subMeshList) {
        SubModel* subModel = new SubModel(this);
        subModel->setSubMesh(subMesh);
        
        mSubModelList[index] = subModel;
        ++index;
    }
}

