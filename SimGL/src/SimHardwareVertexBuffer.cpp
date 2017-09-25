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
    
    _bufferId = 0;
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

void HardwareVertexBuffer::writeData(size_t start, size_t length, const void* source)
{
    if ((start + length) > _sizeInBytes)
    {
        LogManager::getSingleton().error("FMKHardwareVertexBuffer::writeData", "write request out of bounds.");
    }
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    
    if (start ==0 && length == _sizeInBytes)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, _sizeInBytes, source);
    }
    
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, start, length, source);
    }
}


void* HardwareVertexBuffer::_lockImpl(size_t start, size_t length)
{
    unsigned char* buffer;
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    
    buffer = static_cast<unsigned char *>(glMapBufferRange(GL_ARRAY_BUFFER, start, _sizeInBytes, GL_MAP_WRITE_BIT));
    
    _isLocked = true;
    return static_cast<void *>(buffer);
}

void HardwareVertexBuffer::_unlockImpl()
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    _isLocked = false;
}

