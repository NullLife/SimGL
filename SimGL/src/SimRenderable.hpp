//
//  SimRenderable.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimRenderable_hpp
#define SimRenderable_hpp

#include "SimCommon.hpp"
#include "SimSceneManager.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimRenderOperation.hpp"


/** This class including base render informations. eg: material, world matrix, light and so on.
 */
class Renderable {
public:
    Renderable();
    virtual ~Renderable();
    
public:
    virtual const MaterialPtr& getMaterial() = 0;
    
    virtual void getRenderOperation(RenderOperation& op) = 0;
    
    virtual Mat4& getWorldTransforms() = 0;
    
    virtual const Technique* getTechnique();
    
};

#endif /* SimRenderable_hpp */
