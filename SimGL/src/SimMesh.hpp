//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MESH_HPP
#define SIMGL_MESH_HPP

#include "SimCommon.hpp"
#include "SimSubMesh.hpp"

class Mesh {
public:
    typedef Vector<SubMesh*> SubMeshList;
public:
    Mesh(const String& name);
    ~Mesh();

private:
    String mName;
    SubMeshList mSubMeshList;

public:
    const String& getName();

    const int addSubMesh(VertexData* vertexData, IndexData* indexData);
    const int addSubMesh(SubMesh* subMesh);

    const SubMesh* getSubMesh(const int index);
    const SubMeshList& getSubMeshList();
};


#endif //SIMGL_MESH_HPP
