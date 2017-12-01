//
// Created by yangbin on 2017/4/16.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSubModel.hpp"

#include "SimResourceManager.hpp"
#include "SimSceneManager.hpp"
#include "SimVertexArrayManager.hpp"

#include "SimModel.hpp"

#include "SimSubMesh.hpp"

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
    op._obj = this;
    
    op._drawType = mParent->getDrawType();
    op._polyMode = mParent->getPolygonMode();

    op._useIndex = false;
    op._start = 0;
    
    op._vertexData = mSubMesh->getVertexData();
    op._count = op._vertexData->getNumberVertices();
    
    SimUInt64 vaoKey = 0;
    vaoKey = op._vertexData->getBuffer()->getBufferId();
    
    op._indexData = mSubMesh->getIndexData();
    if (op._indexData)
    {
        op._useIndex = true;
        op._count = op._indexData->getNumberIndices();
        
        vaoKey += static_cast<SimUInt64> (op._indexData->getBuffer()->getBufferId()) << 32;
    }
    
    op._vao = VertexArrayManager::getSingleton().getVao(vaoKey);
}
