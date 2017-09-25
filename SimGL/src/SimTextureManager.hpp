//
//  SimTextureManager.hpp
//
//  Created by yangbin on 2017/8/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimTextureManager_hpp
#define SimTextureManager_hpp

#include "SimSingleton.hpp"
#include "SimTexture.hpp"

class TextureManager : public Singleton<TextureManager>
{
public:
    typedef HashMap<String, TexturePtr> TextureCache;
    
    TextureManager();
    ~TextureManager();
    
    TexturePtr get(const String& name);
    
    TexturePtr create(const String& name, const Texture::TextureType type);
    
    bool remove(const String& name);
    
private:
    TextureCache _cache;
};

#endif /* SimTextureManager_hpp */
