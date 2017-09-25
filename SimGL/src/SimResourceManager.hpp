//
// Created by yangbin on 16/9/27.
// Copyright (c) 2016 cleb. All rights reserved.
//

#ifndef SimResourceManager_hpp
#define SimResourceManager_hpp

#include "SimSingleton.hpp"
#include "SimCommon.hpp"

class Texture;

class ResourceManager : public Singleton<ResourceManager> {
public:
    ResourceManager();
    ~ResourceManager();
};


#endif //SimResourceManager_hpp
