//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_RENDEROPERATION_HPP
#define SIMGL_RENDEROPERATION_HPP

#include "SimCommon.hpp"
#include "SimVertexIndexData.hpp"

class Renderable;

enum BufferType
{
    BT_VERTEX = 0,
    BT_INDEX
};

enum PolygonMode
{
    PM_POINT = 0x1B00,
    PM_LINE,
    PM_FILL
};

enum DrawType
{
    DT_POINTS = GL_POINTS,  // 0x0000
    DT_LINES,
    DT_LINE_STRIP,
    DT_LINE_LOOP,
    DT_TRIANGLES,
    DT_TRIANGLE_STRIP,
    DT_TRIANGLE_FAN,
    DT_QUADS,
    DT_QUAD_STRIP,
    DT_POLYGON
};

class RenderOperation
{
public:
    RenderOperation();
    ~RenderOperation();

public:
    Renderable* _obj;
    DrawType _drawType;
    
    PolygonMode _polyMode;
    
    GLuint _vao;
    VertexData* _vertexData;
    IndexData* _indexData;
    bool _useIndex;

    size_t _start;
    size_t _count;
    
    size_t _numOfInstances;
    bool _hasInstancesData;
};


#endif //SIMGL_RENDEROPERATION_HPP
