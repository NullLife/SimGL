//
//  SimRenderer.cpp
//  SimGL
//
//  Created by yangbin on 2017/11/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimRenderer.hpp"

Renderer::Renderer() :
    _activeProgram(nullptr)
{
}

Renderer::~Renderer()
{
}

void Renderer::setActiveProgram(GLProgram* prog)
{
    _activeProgram = prog;
}
