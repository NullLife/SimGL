//
//  SimResource.cpp
//  SimGL
//
//  Created by yangbin on 2017/11/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimResource.hpp"


Resource::Resource(const String& name) :
    _name(name),
    _isLoaded(false)
{
    
}

Resource::~Resource()
{
    
}

const String& Resource::getName() const
{
    return _name;
}
