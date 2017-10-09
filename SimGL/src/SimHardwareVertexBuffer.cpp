//
//  SimHardwareVertexBuffer.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimHardwareVertexBuffer.hpp"
#include "SimLogManager.hpp"

HardwareVertexBuffer::HardwareVertexBuffer(size_t vertexSize, size_t numVertices, Usage usage) :
    HardwareBuffer(usage),
    _numVertices(numVertices),
    _vertexSize(vertexSize)
{
    _sizeInBytes = vertexSize * numVertices;
    
    createBuffer();
}

HardwareVertexBuffer::~HardwareVertexBuffer()
{
    destroyBuffer();
}

void HardwareVertexBuffer::createBuffer()
{
    glGenBuffers(1, &_bufferId);
    if (_bufferId <=0 )
    {
        // error:
    }
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    glBufferData(GL_ARRAY_BUFFER, _sizeInBytes, nullptr, HardwareBuffer::getGLUsage(_usage));
}

void HardwareVertexBuffer::destroyBuffer()
{
    if (_bufferId>0)
    {
        glDeleteBuffers(1, &_bufferId);
        _bufferId = 0;
    }
}

size_t HardwareVertexBuffer::getVertexSize() const
{
    return _vertexSize;
}

size_t HardwareVertexBuffer::getNumVertices() const
{
    return _numVertices;
}

void* HardwareVertexBuffer::_lockImpl(size_t start, size_t length)
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    
    _isLocked = true;
    
    return glMapBufferRange(GL_ARRAY_BUFFER, start, _sizeInBytes, GL_MAP_WRITE_BIT);
}

void HardwareVertexBuffer::_unlockImpl()
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    _isLocked = false;
}

