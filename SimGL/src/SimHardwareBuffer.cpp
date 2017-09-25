//
//  SimHardwareBuffer.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimHardwareBuffer.hpp"
#include "SimLogManager.hpp"

GLenum HardwareBuffer::getGLUsage(Usage usage)
{
    switch (usage)
    {
        case HardwareBuffer::HBU_STATIC:
            return GL_STATIC_DRAW;
        case HardwareBuffer::HBU_DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
        return GL_DYNAMIC_DRAW;
    }
}

HardwareBuffer::HardwareBuffer(Usage usage) :
    _usage(usage),
    _sizeInBytes(0),
    _isLocked(false),
    _lockStart(0),
    _lockSize(0)
{
    
}

HardwareBuffer::~HardwareBuffer()
{
    
}

bool HardwareBuffer::isLocked() const
{
    return _isLocked;
}

void* HardwareBuffer::lock(size_t start, size_t length)
{
    if (_isLocked)
    {
        LogManager::getSingleton().error("HardwareBuffer::lock", "Cannot lock this buffer, it is already locked!");
    }
    
    void* ret = nullptr;
    if ((start + length) > _sizeInBytes)
    {
        LogManager::getSingleton().error( "HardwareBuffer::lock", "Lock request out of bounds.");
    }
    
    ret = _lockImpl(start, length);
    
    _isLocked = true;

    _lockStart = start;
    _lockSize = length;
    
    return ret;
}

void HardwareBuffer::unlock()
{
    if (!_isLocked)
    {
        LogManager::getSingleton().error("HardwareBuffer::unlock", "Cannot unlock this buffer, it is not locked!");
    }
    
    _unlockImpl();
    _isLocked = false;
}
