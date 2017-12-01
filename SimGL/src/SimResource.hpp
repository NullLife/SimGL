//
//  SimResource.hpp
//  SimGL
//
//  Created by yangbin on 2017/11/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimResource_hpp
#define SimResource_hpp

#include "SimCommon.hpp"

class Resource
{
public:
    Resource(const String& name);
    
    virtual ~Resource();
    
    const String& getName() const;
    
    virtual void _load() = 0;
    virtual void _unload() = 0;
    
protected:
    String _name;
    bool _isLoaded;
};

#endif /* SimResource_hpp */
