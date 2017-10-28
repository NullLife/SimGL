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
    mMtlPtr(nullptr)
{
}

SubModel::~SubModel()
{
}

Model* SubModel::getParent()
{
    return mParent;
}

void SubModel::setSubMesh(SubMesh *subMesh)
{
    mSubMesh = subMesh;
}

SubMesh *SubModel::getSubMesh()
{
    return mSubMesh;
}

void SubModel::setMaterial(const MaterialPtr &mtlPtr)
{
    mMtlPtr = mtlPtr;
}

const MaterialPtr& SubModel::getMaterial()
{
    return mMtlPtr;
}

const Mat4& SubModel::getWorldTransforms()
{
    return mParent->getParentNode()->getTransform()->getModelMatrix();
}

void SubModel::setRenderQueueGroup(int queueID)
{
    mQueueID = queueID;
}

const int SubModel::getRenderQueueGroup()
{
    return mQueueID;
}


void SubModel::setVisible(bool visible)
{
    mVisible = visible;
}

bool SubModel::isVisible()
{
    return mVisible;
}

void SubModel::getRenderOperation(RenderOperation& op)
{
    op._drawType = mParent->getDrawType();
    op._polyMode = mParent->getPolygonMode();
    
    op._vertexData = mSubMesh->getVertexData();
    op._indexData = mSubMesh->getIndexData();
    
    op._start = 0;
    
    if (op._indexData)
    {
        op._useIndex = true;
        op._count = op._indexData->getNumIndices();
    }
    else
    {
        op._useIndex = false;
        op._count = op._vertexData->getNumVertices();
    }
}
