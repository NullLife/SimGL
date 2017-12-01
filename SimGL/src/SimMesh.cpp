//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimMesh.hpp"

Mesh::Mesh() :
    Resource("")
{
}

Mesh::Mesh(const String& name) :
    Resource(name)
{
}

Mesh::~Mesh()
{
    LogManager::getSingleton().debug("Delete Mesh", "name:"+ _name +" submesh number: " + std::to_string(_subMeshList.size()));
    for (auto iter=_subMeshList.begin(); iter!=_subMeshList.end(); ++iter)
    {
        delete (*iter);
    }
    _subMeshList.clear();
    _subMeshList.shrink_to_fit();
}

void Mesh::setName(const String& meshName)
{
    _name = meshName;
}

const int Mesh::addSubMesh(VertexData *vertexData, IndexData *indexData)
{
    return addSubMesh(new SubMesh(vertexData, indexData));
}

const int Mesh::addSubMesh(SubMesh *subMesh)
{
    unsigned int index = static_cast<unsigned int>(_subMeshList.size());
    subMesh->setParent(this);
    _subMeshList.push_back(subMesh);
    return index;
}

const SubMesh *Mesh::getSubMesh(const int index)
{
    return _subMeshList[index];
}

const Mesh::SubMeshList &Mesh::getSubMeshList()
{
    return _subMeshList;
}

void Mesh::_load()
{
    if (_isLoaded)
        return;
    
    
}

void Mesh::_unload()
{
}


