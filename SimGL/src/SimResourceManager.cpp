//
// Created by yangbin on 16/9/27.
// Copyright (c) 2016 cleb. All rights reserved.
//

#include "SimResourceManager.hpp"
#include "SimModel.hpp"
#include "SimMesh.hpp"
#include "SimMaterial.hpp"
#include "SimTexture.hpp"
#include "SimPass.hpp"


template<> ResourceManager *Singleton<ResourceManager>::mSingleton = nullptr;

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {

}
