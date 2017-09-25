//
//  SimTextureUnitState.hpp
//
//  Created by yangbin on 2017/8/7.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimTextureUnitState_hpp
#define SimTextureUnitState_hpp

#include "SimTexture.hpp"

class Pass;

enum FilterType
{
    FT_MIN,
    FT_MAG,
    FT_MIP
};
enum FilterOptions
{
    FO_NEAR,
    FO_LINEAR
};
enum TextureWrapMode
{
    TWM_REPEAT,
    TWM_MIRROR,
    TWM_CLAMP
};
struct UVWWrapMode
{
    TextureWrapMode u, v, w;
};

class TextureUnitState {
public:
    TextureUnitState(Pass* parent);
    ~TextureUnitState();
    
    Pass* getParent();
    
    bool is2DTexture() const;
    
    void setTexture(const String& name, const Texture::TextureType type);
    void setTexture(const TexturePtr& tex);
    void setTextureUnit(const unsigned int unit);
    
    const Texture::TextureType getTextureType() const;
    const TexturePtr& getTexture() const;
    const String& getTextureName() const;
    const unsigned int getTextureUnit() const;
    
    void setNumMipmaps(unsigned int num);
    unsigned int getNumMipmaps() const;
    
    void setTextureAlias(const String& texAlias);
    const String& getTextureAlias() const;
    
    void setTextureFiltering(FilterType type, FilterOptions opt);
    void setTextureFiltering(FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter);
    const FilterOptions getTextureFiltering(FilterType type);
    
    void setTextureWrap(TextureWrapMode wrapMode);
    void setTextureWrap(TextureWrapMode u, TextureWrapMode v, TextureWrapMode w);
    void setTextureWrap(const UVWWrapMode& uvw);
    const UVWWrapMode& getTextureWrapMode();
    
    void setAttachment(GLenum attachment);
    GLenum getAttachment() const;
    
    bool _loadTexture(GLenum pixelDataType = GL_UNSIGNED_BYTE, GLenum pixelFormat = GL_RGBA, GLint pixelComponent = GL_RGBA);

private:
    bool _load2DTex(const std::string& filePath, GLenum pixelDataType = GL_UNSIGNED_BYTE, GLenum pixelFormat = GL_RGBA, GLint pixelComponent = GL_RGBA);
    bool _loadCubicTex(const std::string& texName, GLenum pixelDataType = GL_UNSIGNED_BYTE, GLenum pixelFormat = GL_RGBA, GLint pixelComponent = GL_RGBA);
    
    static unsigned char *readImage(const String& imagePath, int* width, int* height, int* channels, int force_channels);
    
private:
    Pass* _parent;
    
    String   _texName;
    String   _texAlias;
    TexturePtr _texture;
    
    unsigned int _texUnit;
    
    unsigned int _textureMipmaps;
    
    GLenum _attachment;
    
    FilterOptions _minFilter;
    FilterOptions _magFilter;
    FilterOptions _mipFilter;
    
    UVWWrapMode _wrapMode;
};

#endif /* SimTextureUnitState_hpp */
