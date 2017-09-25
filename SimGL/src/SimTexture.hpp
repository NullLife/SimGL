//
//  SimTexture.hpp
//
//  Created by yangbin on 2017/8/7.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimTexture_hpp
#define SimTexture_hpp

#include "SimCommon.hpp"

class Texture
{
public:
    enum TextureType
    {
        TEX_TYPE_2D,
        TEX_TYPE_CUBIC
    };
public:
    // The name will be key in cache. so it should be unique. Don't directly use this construct.
    Texture(const String& name, const TextureType type);
    ~Texture();
    
    // Get texture id.
    GLuint getId() const;
    bool isLoaded() const;
    void markLoaded();
    
    // Get texture type. (1D, 2D, 3D, CUBIC...)
    const TextureType getType() const;
    const std::string& getName() const;
    const GLenum getTextureTarget() const;
    
    // Set texture size. if the size is 0, than use the image's size.
    void setWidth(int width);
    void setHeight(int height);
    int getWidth() const;
    int getHeight() const;
    
    void _setSrcWidth(int width);
    void _setSrcHeight(int height);
        
protected:
    GLuint _id;
    bool _isLoaded;
    
    String _name;
    TextureType _type;
    
    int _width;
    int _height;
    int _depth;
    
    int _srcWidth;
    int _srcHeight;
    int _srcDepth;
};

#endif /* SimTexture_hpp */
