//
//  SimHardwareVertexBuffer_hpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimHardwareVertexBuffer_hpp
#define SimHardwareVertexBuffer_hpp

#include "SimHardwareBuffer.hpp"

class HardwareVertexBuffer : public HardwareBuffer
{
public:
    HardwareVertexBuffer(size_t vertexSize, size_t numVertices, Usage usage);
    ~HardwareVertexBuffer();
    
    // Gets the size in bytes of a single vertex in this buffer.
    size_t getVertexSize() const;
    
    // Get the number of vertices in this buffer.
    size_t getNumVertices() const;
    
    const GLuint getBufferId() const { return _bufferId; }
    
private:
    void* _lockImpl(size_t start, size_t length, char writeBit = 1);
    void _unlockImpl();
    
    void createBuffer();
    void destroyBuffer();
    
private:
    size_t _numVertices;
    size_t _vertexSize;
    
    GLuint _bufferId;
};


#endif /* SimHardwareVertexBuffer_hpp */
