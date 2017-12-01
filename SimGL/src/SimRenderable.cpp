//
//  SimRenderable.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimRenderable.hpp"

Renderable::Renderable() :
    _renderer(nullptr)
{}

Renderable::~Renderable()
{}


const Technique* Renderable::getTechnique()
{
    return getMaterial()->getBestTechnique();
}
