//
//  SimRenderer.hpp
//  SimGL
//
//  Created by yangbin on 2017/11/6.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimRenderer_hpp
#define SimRenderer_hpp

#include "SimCommon.hpp"

class GLProgram;
class GLRenderSystem;
class RenderOperation;
class Pass;

/** Using self renderer.
 */
class Renderer
{
public:
    Renderer();
    virtual ~Renderer();
    
    void setActiveProgram(GLProgram* prog);
    
    virtual void render(GLRenderSystem* system, RenderOperation& op, Pass* pass) = 0;
    
protected:
    GLProgram* _activeProgram;
};

#endif /* SimRenderer_hpp */
