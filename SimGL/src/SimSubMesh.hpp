//
// Created by yangbin on 2017/4/16.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SUBMESH_HPP
#define SIMGL_SUBMESH_HPP

#include "SimCommon.hpp"
#include "SimVertexIndexData.hpp"

class Mesh;

class SubMesh {
public:
    SubMesh();
    SubMesh(VertexData* vertexData, IndexData* indexData);
    ~SubMesh();

private:
    VertexData* mVertexData;
    IndexData* mIndexData;
    
    Mesh* mParent;

public:
    void setVertexData(VertexData* vertexData);

    void setIndexData(IndexData* indexData);
    
    void setParent(Mesh* mesh);

    VertexData* getVertexData();

    IndexData* getIndexData();
    
    Mesh* getParent();
};


#endif //SIMGL_SUBMESH_HPP
