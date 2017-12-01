//
//  SimParticleSystemRenderer.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimParticleSystemRenderer_hpp
#define SimParticleSystemRenderer_hpp

#include "SimRenderer.hpp"
#include "SimRenderOperation.hpp"

class ParticleSystem;

class ParticleSystemRenderer : public Renderer
{
    friend class ParticleSystem;
public:
    ParticleSystemRenderer();
    ~ParticleSystemRenderer();
    
    void setMaterial(const String& mtlName);
    const MaterialPtr& getMaterial();
    
    void render(GLRenderSystem* system, RenderOperation& op, Pass* pass);
   
private:
    void startTransfromFeedback(const DrawType drawType);
    void pauseTransfromFeedback();
    void resumeTransfromFeedback();
    void stopTransfromFeedback();
    
    void offRasterizer();
    void onRasterizer();
    
    MaterialPtr _mtl;
};

#endif /* SimParticleSystemRenderer_hpp */
