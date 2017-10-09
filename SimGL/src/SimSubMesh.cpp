//
// Created by yangbin on 2017/4/16.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSubMesh.hpp"

SubMesh::SubMesh() :
        mParent(nullptr),
        mVertexData(nullptr),
        mIndexData(nullptr) {

}

SubMesh::SubMesh(VertexData *vertexData, IndexData *indexData) :
    mParent(nullptr),
    mVertexData(vertexData),
    mIndexData(indexData)
{
}

SubMesh::~SubMesh()
{
    LogManager::getSingleton().debug("Delete SubMesh");
    if (mVertexData)
    {
        delete mVertexData;
        mVertexData = nullptr;
    }
    if (mIndexData)
    {
        delete mIndexData;
        mIndexData = nullptr;
    }
}

void SubMesh::setVertexData(VertexData *vertexData)
{
    mVertexData = vertexData;
}

void SubMesh::setIndexData(IndexData *indexData)
{
    mIndexData = indexData;
}

void SubMesh::setParent(Mesh* mesh)
{
    mParent = mesh;
}

VertexData *SubMesh::getVertexData()
{
    return mVertexData;
}

IndexData *SubMesh::getIndexData()
{
    return mIndexData;
}

Mesh* SubMesh::getParent()
{
    return mParent;
}
