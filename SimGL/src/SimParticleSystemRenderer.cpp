//
//  SimParticleSystemRenderer.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimParticleSystemRenderer.hpp"

#include "SimMaterialManager.hpp"
#include "SimLogManager.hpp"
#include "SimVertexArrayManager.hpp"

#include "SimGLProgram.hpp"
#include "SimPass.hpp"
#include "SimParticleSystem.hpp"

ParticleSystemRenderer::ParticleSystemRenderer() :
    Renderer(),
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

void ParticleSystemRenderer::render(GLRenderSystem* system, RenderOperation& op, Pass* pass)
{
    ParticleSystem* sys = (ParticleSystem *) op._obj;
    
    const SubroutineIndices& subroutines = _activeProgram->getSubroutineIndices();
    
    if (pass->getName() == "update")
    {
        // Set subroutine for update.
        GLuint updatePass = subroutines.at("update");
        
        glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &updatePass);

        sys->_updateParticles(system, op, pass);
        return;
    }
    
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_MULTISAMPLE);
    SIM_CHECK_GL_ERROR( glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE) );
    
    // render
    GLuint renderPass = subroutines.at("render");
    
    glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &renderPass);
    sys->_renderParticles(system, op, pass);
    
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    
//    glDisable(GL_BLEND);
}

void ParticleSystemRenderer::startTransfromFeedback(const DrawType drawType)
{
    SIM_CHECK_GL_ERROR( glBeginTransformFeedback(drawType) );
}

void ParticleSystemRenderer::pauseTransfromFeedback()
{
    glPauseTransformFeedback();
}

void ParticleSystemRenderer::resumeTransfromFeedback()
{
    glResumeTransformFeedback();
}

void ParticleSystemRenderer::stopTransfromFeedback()
{
    SIM_CHECK_GL_ERROR( glEndTransformFeedback() );
}

void ParticleSystemRenderer::offRasterizer()
{
    SIM_CHECK_GL_ERROR(glEnable(GL_RASTERIZER_DISCARD) );
}

void ParticleSystemRenderer::onRasterizer()
{
    SIM_CHECK_GL_ERROR( glDisable(GL_RASTERIZER_DISCARD) );
}

