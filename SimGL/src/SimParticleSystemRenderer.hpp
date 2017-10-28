//
//  SimParticleSystemRenderer.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParticleSystemRenderer_hpp
#define SimParticleSystemRenderer_hpp

#include "SimCommon.hpp"

class ParticleSystemRenderer
{
public:
    ParticleSystemRenderer();
    ~ParticleSystemRenderer();
    
    void setMaterial(const String& mtlName);
    const MaterialPtr& getMaterial();
    
private:
    MaterialPtr _mtl;
};

#endif /* SimParticleSystemRenderer_hpp */
