//
//  SimParticleSystemRenderer.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParticleSystemRenderer.hpp"

#include "SimMaterialManager.hpp"

ParticleSystemRenderer::ParticleSystemRenderer() :
    _mtl(nullptr)
{
}

ParticleSystemRenderer::~ParticleSystemRenderer()
{
    
}

void ParticleSystemRenderer::setMaterial(const String& mtlName)
{
    _mtl = MaterialManager::getSingleton().get(mtlName);
}

const MaterialPtr& ParticleSystemRenderer::getMaterial()
{
    return _mtl;
}
