//
//  SimTexture.cpp
//
//  Created by yangbin on 2017/8/7.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimTexture.hpp"

#include "SOIL.h"

#include "SimLogManager.hpp"

Texture::Texture(const String& name, const TextureType type) :
    _name(name),
    _type(type),
    _id(0),
    _isLoaded(false),
    _width(0),
    _height(0),
    _depth(0),
    _srcWidth(0),
    _srcHeight(0),
    _srcDepth(0)
{
    glGenTextures(1, &_id);
}

Texture::~Texture()
{
    if (_id > 0)
    {
        glDeleteTextures(1, &_id);
    }
    _id = 0;
    _isLoaded = false;
}

GLuint Texture::getId() const
{
    return _id;
}

bool Texture::isLoaded() const
{
    return _isLoaded;
}

void Texture::markLoaded()
{
    _isLoaded = true;
}

const Texture::TextureType Texture::getType() const
{
    return _type;
}

const GLenum Texture::getTextureTarget() const
{
    switch (_type) {
        case TEX_TYPE_2D:
            return GL_TEXTURE_2D;
        case TEX_TYPE_CUBIC:
            return GL_TEXTURE_CUBE_MAP;
    }
    return GL_TEXTURE_2D;
}

const String& Texture::getName() const
{
    return _name;
}

void Texture::setWidth(int width)
{
    _width = width;
}
int Texture::getWidth() const
{
    return _width;
}

void Texture::setHeight(int height)
{
    _height = height;
}
int Texture::getHeight() const
{
    return _height;
}

void Texture::_setSrcWidth(int width)
{
    _srcWidth = width;
}

void Texture::_setSrcHeight(int height)
{
    _srcHeight = height;
}
