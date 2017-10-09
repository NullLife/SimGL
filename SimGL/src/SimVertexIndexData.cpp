//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimVertexIndexData.hpp"

#include "SimHardwareVertexBuffer.hpp"
#include "SimHardwareIndexBuffer.hpp"

VertexElement::VertexElement(const VertexElementSemantic semantic, const VertexElementType type) :
    mSemantic(semantic),
    mType(type)
{
}

VertexElement::~VertexElement()
{
    LogManager::getSingleton().debug("Delete VertexElement", std::to_string((long)this) + " semantic: " + std::to_string(mSemantic));
}

const VertexElementSemantic VertexElement::getVertexElementSemantic() const
{
    return mSemantic;
}

VertexElementType VertexElement::getVertexElementType() const
{
    return mType;
}

size_t VertexElement::getVertexElementOffset(const VertexElementType type)
{
    size_t offset = 0;
    switch (type) {
        case VET_FLOAT3:
            offset = sizeof(float) * 3;
            break;
        case VET_FLOAT4:
            offset = sizeof(float) * 4;
            break;
        case VET_FLOAT2:
            offset = sizeof(float) * 2;
            break;
        case VET_FLOAT1:
            offset = sizeof(float) * 1;
    }
    return offset;
}

size_t VertexElement::getVertexElementComponentCount(const VertexElementType type)
{
    size_t offset = 0;
    switch (type)
    {
        case VET_FLOAT3:
            offset = 3;
            break;
        case VET_FLOAT4:
            offset = 4;
            break;
        case VET_FLOAT2:
            offset = 2;
            break;
        case VET_FLOAT1:
            offset = 1;
    }
    return offset;
}

////////////////////////////////////////////////

VertexDataDeclare::VertexDataDeclare() :
    mStride(0),
    mNumber(0),
    mComponentCount(0)
{
}

VertexDataDeclare::~VertexDataDeclare()
{
    LogManager::getSingleton().debug("Delete VertexDataDeclare");
    VertexElements::iterator iter = mEles.begin();
    VertexElements::const_iterator enditer = mEles.end();
    for (; iter != enditer; ++iter)
    {
        delete *iter;
    }
    mEles.clear();
    mEles.shrink_to_fit();
}

const VertexDataDeclare::VertexElements & VertexDataDeclare::getVertexElements() const
{
    return mEles;
}

void VertexDataDeclare::addElement(VertexElement* ele)
{
    auto it = mEles.begin();
    while (it != mEles.end())
    {
        if ((*it)->getVertexElementSemantic() == ele->getVertexElementSemantic())
        {
            LogManager::getSingleton().debug("VertexDataDeclare::addElement", "A vertex semantic was declared twice!");
            return;
        }
        ++it;
    }
    mEles.push_back(ele);
    mNumber++;
    mStride += VertexElement::getVertexElementOffset(ele->getVertexElementType());
    mComponentCount += VertexElement::getVertexElementComponentCount(ele->getVertexElementType());
}

const size_t &VertexDataDeclare::getNumber() const
{
    return mNumber;
}

const size_t &VertexDataDeclare::getStride() const
{
    return mStride;
}

const int VertexDataDeclare::getComponentCount() const
{
    return mComponentCount;
}

////////////////////////////////////////////////

VertexData::VertexData(VertexDataDeclare* dataDeclaration) :
    mDeclaration(dataDeclaration),
    mVerBuffer(nullptr),
    _isBinded(false)
{
}

VertexData::~VertexData()
{
    LogManager::getSingleton().debug("Delete VertexData", "vertex number: " + std::to_string(mVerBuffer->getNumVertices()));
    if (mDeclaration)
    {
        delete mDeclaration;
        mDeclaration = nullptr;
    }
    
    if (mVerBuffer)
    {
        delete mVerBuffer;
        mVerBuffer = nullptr;
    }
}

void VertexData::initBuffer(size_t vertexSize, size_t numVertices, HardwareBuffer::Usage usage)
{
    mVerBuffer = new HardwareVertexBuffer(vertexSize, numVertices, usage);
}

///////////////////////////////////////////////

IndexData::IndexData() :
    mIdxBuffer(nullptr)
{
}

IndexData::~IndexData()
{
    LogManager::getSingleton().debug("Delete VertexIndexData", "indices number: " + std::to_string(mIdxBuffer->getNumIndices()));
    if (mIdxBuffer)
    {
        delete mIdxBuffer;
        mIdxBuffer = nullptr;
    }
}

void IndexData::initBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndices, HardwareBuffer::Usage usage)
{
    mIdxBuffer = new HardwareIndexBuffer(itype, numIndices, usage);
}
