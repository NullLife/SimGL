//
//  SimMaterialManager.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimMaterialManager.hpp"

#include "SimTextureManager.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"
#include "SimTextureUnitState.hpp"

#include "SimScriptCompiler.hpp"


template<> MaterialManager *Singleton<MaterialManager>::mSingleton = nullptr;

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
    mCache.clear();
}

MaterialPtr MaterialManager::get(const String& name)
{
    auto iter = mCache.find(name);
    if (iter == mCache.end())
    {
        // Load material from script
        if (!_load(name))
        {
            LogManager::getSingleton().debug("Fail to load material", name);
            return nullptr;
        }
        return mCache.find(name)->second;
    }
    return iter->second;
}

MaterialPtr MaterialManager::add(Material *mtl)
{
    MaterialPtr ptr(mtl);
    mCache.insert(MaterialCache::value_type(mtl->getName(), ptr));
    return ptr;
}

bool MaterialManager::remove(const std::string &name)
{
    auto iter = mCache.find(name);
    if (iter == mCache.end())
    {
        LogManager::getSingleton().debug("Did not exit the material", name);
        return false;
    }
    mCache.erase(iter);
    return true;
}

bool MaterialManager::_load(const String& name)
{
    return ScriptCompilerManager::getSingleton().compileScript(name);
}
