//
// Created by yangbin on 2017/4/16.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSubModel.hpp"
#include "SimModel.hpp"
#include "SimSceneManager.hpp"
#include "SimSubMesh.hpp"
#include "SimResourceManager.hpp"
#include "SimMaterial.hpp"
#include "SimPass.hpp"
#include "SimShader.hpp"
#include "SimShaderManager.hpp"
#include "SimTransform.hpp"

SubModel::SubModel(Model* parent) :
        Renderable(),
        mParent(parent),
        mQueueID(0),
        mVisible(true),
        mSubMesh(nullptr),
        mMtlPtr(nullptr) {
}

SubModel::~SubModel() {
    
}

Model* SubModel::getParent() {
    return mParent;
}

void SubModel::setSubMesh(SubMesh *subMesh) {
    mSubMesh = subMesh;
}

SubMesh *SubModel::getSubMesh() {
    return mSubMesh;
}

void SubModel::setMaterial(const MaterialPtr &mtlPtr) {
    mMtlPtr = mtlPtr;
}

const MaterialPtr& SubModel::getMaterial() {
    return mMtlPtr;
}

Mat4& SubModel::getWorldTransforms() {
    return mParent->getParentNode()->getTransform()->getMatrix();
}

void SubModel::setRenderQueueGroup(int queueID) {
    mQueueID = queueID;
}

const int SubModel::getRenderQueueGroup() {
    return mQueueID;
}


void SubModel::setVisible(bool visible) {
    mVisible = visible;
}

bool SubModel::isVisible() {
    return mVisible;
}

void SubModel::getRenderOperation(RenderOperation& op) {
    op.mVertexData = mSubMesh->getVertexData();
    op.mVerNum = op.mVertexData->getBuffer()->getNumVertices();
    op.mIndexData = mSubMesh->getIndexData();
    op.mIndexNum = op.mIndexData->getBuffer()->getNumIndices();
    op.mUseIndex = true;
}
