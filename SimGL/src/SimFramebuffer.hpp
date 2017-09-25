//
//  SimFramebuffer.hpp
//  SimGL
//
//  Created by yangbin on 2017/5/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "SimCommon.hpp"

class Texture;

class Framebuffer
{
public:
    Framebuffer(const String& name);
    ~Framebuffer();
    
private:
    String mName;
    GLuint mFrameBuffer;
    GLuint mRenderBuffer;
    
public:
    const String& getName() const;
    
    void initRenderTarget(Texture** textures, int texNum);
    
    void bind();
    
    void unbind(const int width, const int height);
};

#endif /* Framebuffer_hpp */
