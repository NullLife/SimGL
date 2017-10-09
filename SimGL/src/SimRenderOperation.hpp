//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_RENDEROPERATION_HPP
#define SIMGL_RENDEROPERATION_HPP

#include "SimCommon.hpp"
#include "SimVertexIndexData.hpp"

class Transform;
class Renderable;

enum BufferType
{
    BT_VERTEX = 0,
    BT_INDEX
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
    GLuint mVao;
    
    VertexData* mVertexData;
    IndexData* mIndexData;
    bool mUseIndex;
    
    Renderable* mSrcRend;

    DrawType mDrawType;

    size_t mVerNum;
    size_t mIndexStart;
    size_t mIndexNum;
    
    Transform* mTrans;
};


#endif //SIMGL_RENDEROPERATION_HPP
