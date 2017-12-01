//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MESH_HPP
#define SIMGL_MESH_HPP

#include "SimResource.hpp"
#include "SimSubMesh.hpp"

class Mesh : public Resource
{
public:
    typedef Vector<SubMesh*> SubMeshList;
    
    Mesh();
    Mesh(const String& name);
    ~Mesh();
    
    // Set mesh name.
    void setName(const String& meshName);

    // Add submesh with data.
    const int addSubMesh(VertexData* vertexData, IndexData* indexData);
    const int addSubMesh(SubMesh* subMesh);

    const SubMesh* getSubMesh(const int index);
    const SubMeshList& getSubMeshList();
    
    // Load mesh resource.
    void _load();
    
    // Unload mesh resource.
    void _unload();
    
private:
    
    SubMeshList _subMeshList;
};


#endif //SIMGL_MESH_HPP
