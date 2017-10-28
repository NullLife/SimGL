//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimRenderOperation.hpp"

RenderOperation::RenderOperation() :
    _vao(0),
    _vertexData(nullptr),
    _indexData(nullptr),
    _obj(nullptr),
    _drawType(DT_TRIANGLES),
    _polyMode(PM_FILL),
    _useIndex(true),
    _start(0),
    _hasInstancesData(false),
    _numOfInstances(0)
{
}

RenderOperation::~RenderOperation()
{
}
