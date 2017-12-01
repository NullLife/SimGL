//
//  SimHardwareVertexBuffer.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimHardwareVertexBuffer.hpp"
#include "SimLogManager.hpp"

HardwareVertexBuffer::HardwareVertexBuffer(size_t bufSize, Usage usage) :
    HardwareBuffer(usage)
{
    _sizeInBytes = bufSize;
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

void* HardwareVertexBuffer::_lockImpl(size_t start, size_t length, char writeBit)
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    
    _isLocked = true;
    
    if (writeBit == 1)
        return glMapBufferRange(GL_ARRAY_BUFFER, start, length, GL_MAP_WRITE_BIT);
    else
        return glMapBufferRange(GL_ARRAY_BUFFER, start, length, GL_MAP_READ_BIT);
}

void HardwareVertexBuffer::_unlockImpl()
{
    glBindBuffer(GL_ARRAY_BUFFER, _bufferId);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    _isLocked = false;
}

