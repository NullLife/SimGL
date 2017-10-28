//
//  SimHardwareIndexBuffer.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimHardwareIndexBuffer.hpp"
#include "SimLogManager.hpp"

size_t HardwareIndexBuffer::calculateIndexSize(IndexType type)
{
    if (type == IT_UInt)
    {
        return sizeof(unsigned int);
    }
    else if (type == IT_UShort)
    {
        return sizeof(unsigned short);
    }
    return 0;
}

HardwareIndexBuffer::HardwareIndexBuffer(IndexType itype, size_t numIndices, Usage usage) :
    HardwareBuffer(usage),
    _type(itype),
    _numIndices(numIndices)
{
    _indexSize = HardwareIndexBuffer::calculateIndexSize(itype);
    _sizeInBytes =  _indexSize* _numIndices;
    
    createBuffer();
}

HardwareIndexBuffer::~HardwareIndexBuffer()
{
    destroyBuffer();
}

HardwareIndexBuffer::IndexType HardwareIndexBuffer::getIndexType() const
{
    return _type;
}

size_t HardwareIndexBuffer::getNumIndices() const
{
    return _numIndices;
}

size_t HardwareIndexBuffer::getIndexSize() const
{
    return _indexSize;
}

void HardwareIndexBuffer::createBuffer()
{
    glGenBuffers(1, &_bufferId);
    if (_bufferId <=0 )
    {
        // error:
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _sizeInBytes, nullptr, HardwareBuffer::getGLUsage(_usage));
}

void HardwareIndexBuffer::destroyBuffer()
{
    if (_bufferId>0)
    {
        glDeleteBuffers(1, &_bufferId);
        _bufferId = 0;
    }
}

void* HardwareIndexBuffer::_lockImpl(size_t start, size_t length, char writeBit)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
    
    _isLocked = true;
    
    if (writeBit == 1)
        return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, start, length, GL_MAP_WRITE_BIT);
    else
        return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, start, length, GL_MAP_READ_BIT);
}

void HardwareIndexBuffer::_unlockImpl()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferId);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    _isLocked = false;
}
