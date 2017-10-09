//
//  FMKTextureUnitState.cpp
//  Fengmap_Kernel_v30
//
//  Created by yangbin on 2017/8/7.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimTextureUnitState.hpp"

#include "SOIL.h"

#include "SimTextureManager.hpp"
#include "SimLogManager.hpp"
#include "SimCommon.hpp"

unsigned char *TextureUnitState::readImage(const String& imagePath, int* width, int* height, int* channels, int force_channels)
{
    String filePath = TEXTURE_RESOURCE_DIR + imagePath;
    unsigned char *data = SOIL_load_image(filePath.c_str(), width, height, channels, force_channels);
    
    if (data == nullptr) {
        LogManager::getSingleton().debug("Texture::readImage", "failed to load " + imagePath);
    }
    return data;
}

TextureUnitState::TextureUnitState(Pass* parent) :
    _parent(parent),
    _texture(NULL),
    _texUnit(0),
    _attachment(GL_COLOR_ATTACHMENT0),
    _texName(""),
    _texAlias(""),       // 正常的纹理贴图
    _minFilter(FO_LINEAR),
    _magFilter(FO_LINEAR),
    _mipFilter(FO_NONE),
    _textureMipmaps(0)   // 不需要产生Mipmap
{
    _wrapMode.u = TWM_CLAMP;
    _wrapMode.v = TWM_CLAMP;
    _wrapMode.w = TWM_CLAMP;
}

TextureUnitState::~TextureUnitState()
{
}

Pass* TextureUnitState::getParent()
{
    return _parent;
}

bool TextureUnitState::is2DTexture() const
{
    return _texture->getType() == Texture::TEX_TYPE_2D;
}

bool TextureUnitState::isCubicTexture() const
{
    return _texture->getType() == Texture::TEX_TYPE_CUBIC;
}

void TextureUnitState::setTexture(const String& name, const Texture::TextureType type)
{
    _texName = name;
    
    // Try get it from cache.
    _texture = TextureManager::getSingleton().get(name);
    if (_texture != NULL)
    {
        // FixMe: Considering reload resource...?
        return;
    }
    
    // Create new texture.
    _texture = TextureManager::getSingleton().create(name, type);
    
    // Using lazy loading.
//    if (type == FMKTexture::TextureType::TEX_TYPE_2D)
//    {
//        _load2DTex(name);
//    }
}

void TextureUnitState::setTexture(const TexturePtr& tex)
{
    _texture = tex;
}

void TextureUnitState::setTextureUnit(const unsigned int unit)
{
    _texUnit = unit;
}

const Texture::TextureType TextureUnitState::getTextureType() const
{
    return _texture->getType();
}

const TexturePtr& TextureUnitState::getTexture() const
{
    return _texture;
}

const String& TextureUnitState::getTextureName() const
{
    return _texName;
}

const unsigned int TextureUnitState::getTextureUnit() const
{
    return _texUnit;
}

void TextureUnitState::setNumMipmaps(unsigned int num)
{
    _textureMipmaps = num;
}

unsigned int TextureUnitState::getNumMipmaps() const
{
    return _textureMipmaps;
}

void TextureUnitState::setTextureAlias(const String& texAlias)
{
    _texAlias = texAlias;
}

const std::string& TextureUnitState::getTextureAlias() const
{
    return _texAlias;
}

void TextureUnitState::setTextureFiltering(FilterType type, FilterOptions opt)
{
    switch (type) {
        case FT_MIN:
            _minFilter = opt;
            break;
        case FT_MAG:
            _magFilter = opt;
            break;
        case FT_MIP:
            _mipFilter = opt;
            break;
        default:
            break;
    }
}

void TextureUnitState::setTextureFiltering(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter)
{
    _mipFilter = minFilter;
    _magFilter = magFilter;
    _mipFilter = mipFilter;
}

const FilterOptions TextureUnitState::getTextureFiltering(FilterType type)
{
    FilterOptions opt;
    switch (type) {
        case FT_MIN:
            opt = _minFilter;
            break;
        case FT_MAG:
            opt = _magFilter;
            break;
        case FT_MIP:
            opt = _mipFilter;
            break;
        default:
            break;
    }
    return opt;
}

void TextureUnitState::setTextureWrap(TextureWrapMode wrapMode)
{
    _wrapMode.u = wrapMode;
    _wrapMode.v = wrapMode;
    _wrapMode.w = wrapMode;
}

void TextureUnitState::setTextureWrap(TextureWrapMode u, TextureWrapMode v, TextureWrapMode w)
{
    _wrapMode.u = u;
    _wrapMode.v = v;
    _wrapMode.w = w;
}

void TextureUnitState::setTextureWrap(const UVWWrapMode& uvw)
{
    _wrapMode = uvw;
}

const UVWWrapMode& TextureUnitState::getTextureWrapMode()
{
    return _wrapMode;
}

void TextureUnitState::setAttachment(GLenum attachment)
{
    _attachment = attachment;
}

GLenum TextureUnitState::getAttachment() const
{
    return _attachment;
}

bool TextureUnitState::_loadTexture(GLenum pixelDataType, GLenum pixelFormat, GLint pixelComponent)
{
    if (_texture->isLoaded())
    {
        return true;
    }
    
    bool ret = false;
    
    switch (_texture->getType())
    {
        case Texture::TEX_TYPE_2D:
            ret = _load2DTex(_texName, pixelDataType, pixelFormat, pixelComponent);
            break;
            
        case Texture::TEX_TYPE_CUBIC:
            ret = _loadCubicTex(_texName, pixelDataType, pixelFormat, pixelComponent);
            break;
            
        default:
            break;
    }
    
    if (ret)
    {
        // Marking it for being loaded.
        _texture->markLoaded();
    }
    return ret;
}

bool TextureUnitState::_load2DTex(const std::string& filePath, GLenum pixelDataType, GLenum pixelFormat, GLint pixelComponent)
{
    // Bind texture.
    glBindTexture(GL_TEXTURE_2D, _texture->getId());
    
    // Read image data.
    int width, height, channels;
    unsigned char *data = readImage(filePath, &width, &height, &channels, SOIL_LOAD_RGBA);
    if (data == NULL)
        return false;
    
    // Set texture content.
    glTexImage2D(GL_TEXTURE_2D, 0, pixelComponent, width, height, 0, pixelFormat, pixelDataType, data);
    
    // Free image.
    SOIL_free_image_data(data);
    
    // Set texture size
    _texture->_setSrcWidth(width);
    _texture->_setSrcHeight(height);
        
    return true;
}

bool TextureUnitState::_loadCubicTex(const String& texName, GLenum pixelDataType, GLenum pixelFormat, GLint pixelComponent)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture->getId());
    
    // Read image data.
    int width, height, channels;
    Vector<String> names;
    StringUtils::split(names, texName, ",");

    for (int i=0; i<names.size(); ++i)
    {
        unsigned char *data = readImage(names[i], &width, &height, &channels, SOIL_LOAD_RGBA);
        
        if (data == NULL)
            return false;
        
        // POSITIVE_X, NEGATIVE_X, POSITIVE_Y, NEGATIVE_Y, POSITIVE_Z, NEGATIVE_Z
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, pixelComponent, width, height, 0, pixelFormat, pixelDataType, data);
        
        SOIL_free_image_data(data);
    }
    
    return true;
}

