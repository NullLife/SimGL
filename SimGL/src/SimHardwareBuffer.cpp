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
        case HardwareBuffer::HBU_READ_ONLY:
            return GL_READ_ONLY;
        case HardwareBuffer::HBU_READ_WRITE:
            return GL_READ_WRITE;

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

void HardwareBuffer::writeData(const void *source)
{
    unsigned char *dst = static_cast<unsigned char *>(lock(1));
    std::memcpy(dst, source, _sizeInBytes);
    unlock();
}

void HardwareBuffer::writeData(size_t start, size_t length, const void *source)
{
    unsigned char *dst = static_cast<unsigned char *>(lock(start, length, 1));
    dst += start;
    std::memcpy(dst, source, length);
    unlock();
}

void HardwareBuffer::readData(size_t start, size_t length, size_t stride, size_t offset,  size_t readLen, void* result)
{
    unsigned char *dst = static_cast<unsigned char *>(lock(start, length, 0));
    size_t pos = start + offset;
    size_t limit = length;
    unsigned char *r = static_cast<unsigned char *>(result);
    size_t index = 0;
    while (pos < limit)
    {
        std::memcpy(r, dst + pos, readLen);
        r += readLen;
        ++index;
        pos = (index*stride + offset);
    }
    unlock();
}

bool HardwareBuffer::isLocked() const
{
    return _isLocked;
}

void* HardwareBuffer::lock(char writeBit)
{
    return lock(0, _sizeInBytes, writeBit);
}

void* HardwareBuffer::lock(size_t start, size_t length, char writeBit)
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
    
    ret = _lockImpl(start, length, writeBit);
    
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
