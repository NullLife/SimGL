//
//  SimTextureManager.cpp
//
//  Created by yangbin on 2017/8/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimTextureManager.hpp"

#include "SimLogManager.hpp"

template<> TextureManager *Singleton<TextureManager>::mSingleton = NULL;
TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    _cache.clear();
}

TexturePtr TextureManager::get(const String& name)
{
    TextureCache::const_iterator it =  _cache.find(name);
    if (it != _cache.end())
    {
        return it->second;
    }
    return NULL;
}

TexturePtr TextureManager::create(const String& name, const Texture::TextureType type)
{
    TexturePtr tex = NULL;
    tex = get(name);
    if (tex != NULL)
    {
        LogManager::getSingleton().error("FMKMaterialManager::create", "Can't create the already existed materal named " + name);
    }
    tex = TexturePtr(new Texture(name, type));
    
    _cache[name] = tex;
    
    return tex;
}

bool TextureManager::remove(const String& name)
{
    TextureCache::const_iterator it =  _cache.find(name);
    if (it != _cache.end())
    {
        _cache.erase(it);
        
        return true;
    }
    return  false;
}
