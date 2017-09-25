//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimMesh.hpp"

Mesh::Mesh(const String& name) :
        mName(name) {
}


Mesh::~Mesh() {
    for (auto iter=mSubMeshList.begin(); iter!=mSubMeshList.end(); ++iter) {
        delete (*iter);
    }
    mSubMeshList.clear();
    mSubMeshList.shrink_to_fit();
    
    LogManager::getSingleton().debug("Delete Mesh", "name:"+ mName +" submesh number: " + std::to_string(mSubMeshList.size()));
}

const String &Mesh::getName() {
    return mName;
}

const int Mesh::addSubMesh(VertexData *vertexData, IndexData *indexData) {
    return addSubMesh(new SubMesh(vertexData, indexData));
}

const int Mesh::addSubMesh(SubMesh *subMesh) {
    const unsigned int index = static_cast<const unsigned int>(mSubMeshList.size());
    subMesh->setParent(this);
    mSubMeshList.push_back(subMesh);
    return index;
}

const SubMesh *Mesh::getSubMesh(const int index) {
    return mSubMeshList[index];
}

const Mesh::SubMeshList &Mesh::getSubMeshList() {
    return mSubMeshList;
}

