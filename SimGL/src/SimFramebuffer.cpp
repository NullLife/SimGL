//
//  SimFramebuffer.cpp
//  SimGL
//
//  Created by yangbin on 2017/5/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimFramebuffer.hpp"
#include "SimTexture.hpp"

Framebuffer::Framebuffer(const String& name) :
    mName(name),
    mFrameBuffer(0),
    mRenderBuffer(0) {
}

Framebuffer::~Framebuffer() {
    if (mFrameBuffer) {
        glDeleteFramebuffers(1, &mFrameBuffer);
        mFrameBuffer = 0;
    }
    if(mRenderBuffer) {
        glDeleteRenderbuffers(1, &mRenderBuffer);
        mRenderBuffer = 0;
    }
}

const String& Framebuffer::getName() const {
    return mName;
}

void Framebuffer::initRenderTarget(Texture** textures, int texNum) {
    if (textures == nullptr || texNum ==0) {
        return;
    }
    
    GLenum drawBuffers[32];      //32 is the max number of bound textures in OpenGL
    assert(texNum <= 32);           //Assert to be sure no buffer overrun should occur
    
    bool hasDepth = false;
    for (int i=0; i<texNum; ++i) {
        Texture *tex = textures[i];
//        if (tex->getAttachment() == 0 || tex->getAttachment() == GL_NONE) {
//            continue;
//        }
//        if (tex->getAttachment() == GL_DEPTH_ATTACHMENT) {
//            drawBuffers[i] = GL_NONE;
//            hasDepth = true;
//        } else {
//            drawBuffers[i] = tex->getAttachment();
//        }
        
        if (mFrameBuffer == 0) {
            glGenFramebuffers(1, &mFrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
        }

        // Bind texture to framebuffer
//        glFramebufferTexture2D(GL_FRAMEBUFFER,
//                                            tex->getAttachment(),
//                                            tex->getTextureTarget(),
//                                            tex->getId(),
//                                            0);
    }
    
    if (mFrameBuffer == 0) {
        return;
    }
    
    if (!hasDepth) {
        glGenRenderbuffers(1, &mRenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, mRenderBuffer);
        // Allocate storage
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH_COMPONENT,
                              textures[0]->getWidth(),
                              textures[0]->getHeight());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  mRenderBuffer);
    }
    
    glDrawBuffers(texNum, drawBuffers);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LogManager::getSingleton().debug("Framebuffer::initRenderTarget", "Framebuffer creation failed!");
        assert(false);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    glViewport(0, 0, 512, 512);
}

void Framebuffer::unbind(const int width, const int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
}

