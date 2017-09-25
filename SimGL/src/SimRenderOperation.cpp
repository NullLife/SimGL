//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimRenderOperation.hpp"

RenderOperation::RenderOperation() :
        mVao(0),
        mVertexData(nullptr),
        mIndexData(nullptr),
        mSrcRend(nullptr),
        mDrawType(DT_TRIANGLES),
        mUseIndex(true),
        mVerNum(0),
        mIndexNum(0),
        mIndexStart(0),
        mTrans(nullptr) {
}

RenderOperation::~RenderOperation() {
}
