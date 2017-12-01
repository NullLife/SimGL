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
    HardwareVertexBuffer(size_t bufSize, Usage usage);
    ~HardwareVertexBuffer();
    
    const GLuint getBufferId() const { return _bufferId; }
    
private:
    void* _lockImpl(size_t start, size_t length, char writeBit = 1);
    void _unlockImpl();
    
    void createBuffer();
    void destroyBuffer();
    
private:
    GLuint _bufferId;
};


#endif /* SimHardwareVertexBuffer_hpp */
