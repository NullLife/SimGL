//
//  FMKHardwareBuffer.hpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimHardwareBuffer_hpp
#define SimHardwareBuffer_hpp

#include "SimCommon.hpp"

class HardwareBuffer {
public:
    enum Usage {
        HBU_STATIC,
        HBU_DYNAMIC,
        HBU_WRITE_ONLY,
        HBU_READ_ONLY,
        HBU_READ_WRITE
    };
    
    static GLenum getGLUsage(Usage usage);
    
public:
    HardwareBuffer(Usage usage);
    virtual ~HardwareBuffer();
    
    virtual size_t getSize() const { return _sizeInBytes; }
    
    virtual bool isLocked() const;
    
    virtual void* lock(size_t start, size_t length);
    virtual void unlock();
    
    virtual const GLuint getBufferId() const = 0;
    virtual void writeData(size_t start, size_t length, const void* source) = 0;
    
protected:
    virtual void* _lockImpl(size_t start, size_t length) = 0;
    virtual void _unlockImpl() = 0;

protected:
    Usage _usage;
    
    size_t _sizeInBytes;
    
    bool _isLocked;
    size_t _lockStart;
    size_t _lockSize;
};




#endif /* SimHardwareBuffer_hpp */
