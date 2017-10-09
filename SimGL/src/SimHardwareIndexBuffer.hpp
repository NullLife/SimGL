//
//  FMKHardwareIndexBuffer.hpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimHardwareIndexBuffer_hpp
#define SimHardwareIndexBuffer_hpp

#include "SimHardwareBuffer.hpp"

class HardwareIndexBuffer : public HardwareBuffer
{
public:
    enum IndexType
    {
        IT_UShort,
        IT_UInt
    };
    
    static size_t calculateIndexSize(IndexType type);
    
public:
    HardwareIndexBuffer(IndexType itype, size_t numIndices, Usage usage);
    ~HardwareIndexBuffer();
    
    // Gets the type of index.
    IndexType getIndexType() const;
    
    // Get the number of index in this buffer.
    size_t getNumIndices() const;
    
    // Get size of a single index.
    size_t getIndexSize() const;
    
    const GLuint getBufferId() const { return _bufferId; }
    
private:
    void createBuffer();
    void destroyBuffer();
    
    void* _lockImpl(size_t start, size_t length);
    void _unlockImpl();
    
private:
    IndexType _type;
    size_t _numIndices;
    size_t _indexSize;
    
    GLuint _bufferId;
};

#endif /* SimHardwareIndexBuffer_hpp */
