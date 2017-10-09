//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimGLRenderSystem.hpp"

#include "SimGLProgramManager.hpp"
#include "SimVertexArrayManager.hpp"

#include "SimPass.hpp"
#include "SimGLShader.hpp"
#include "SimGLProgram.hpp"
#include "SimGLShaderParams.hpp"
#include "SimTextureUnitState.hpp"
#include "SimParameterDataSource.hpp"

#include "SimRenderOperation.hpp"
#include "SimRenderable.hpp"

GLRenderSystem::GLRenderSystem() :
    _currVertShader(nullptr),
    _currGeoShader(nullptr),
    _currFragShader(nullptr),
    _sceneManager(nullptr)
{
}

GLRenderSystem::~GLRenderSystem()
{
}

void GLRenderSystem::setSceneManager(SceneManager* sm)
{
    _sceneManager = sm;
}

void GLRenderSystem::bindVertexShader(GLShader* verShader)
{
    if (_currVertShader != verShader)
    {
        _currVertShader = verShader;
        GLProgramManager::getSingleton().setActiveVertexShader(verShader);
    }
}

void GLRenderSystem::bindGeometryShader(GLShader *geoShader)
{
    if (_currGeoShader != geoShader)
    {
        _currGeoShader = geoShader;
        GLProgramManager::getSingleton().setActiveGeometryShader(geoShader);
    }
}

void GLRenderSystem::bindFragmentShader(GLShader *fragShader)
{
    if (_currFragShader != fragShader)
    {
        _currFragShader = fragShader;
        GLProgramManager::getSingleton().setActiveFragmentShader(fragShader);
    }
}

void GLRenderSystem:: render(RenderOperation &op, Pass* pass)
{
    // Get active program.
    GLProgram* activeProgram = GLProgramManager::getSingleton().getActiveProgram();
    
    // Commit vertex data to GPU.
    _commitVertexData(op);
    
    updateProgramParameters(pass);
    
    // update uniforms.
    activeProgram->updateUniforms();

    // Set texture.
    Pass::TextureUnitStateList& texUnitStates = pass->getTextureUnitStateList();
    for (int i=0; i<texUnitStates.size(); ++i)
    {
        setTextureUnitSettings(texUnitStates[i]);
    }
    
    glDrawElements(op.mDrawType, (GLsizei) op.mIndexNum, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}

void GLRenderSystem::_commitVertexData(RenderOperation &op)
{
    SimUInt64 vaoKey = op.mVertexData->getBuffer()->getBufferId();
    vaoKey += static_cast<SimUInt64> (op.mIndexData->getBuffer()->getBufferId()) << 32;
    op.mVao = VertexArrayManager::getSingleton().getVao(vaoKey);
    
    if (op.mVao == 0)
    {
        LogManager::getSingleton().error("Vertex array id can't be 0 !");
    }
    
    glBindVertexArray(op.mVao);
    // Here, binding the buffer is the most important.
    glBindBuffer(GL_ARRAY_BUFFER, op.mVertexData->getBuffer()->getBufferId());
    
    // Bind Vertex
    if (op.mVertexData->isBinded())
    {
        return;
    }
    
    const VertexDataDeclare* declare = op.mVertexData->getVertexDataDeclare();
    const VertexDataDeclare::VertexElements& ves = declare->getVertexElements();
    for (unsigned int num=0; num<declare->getNumber(); ++num)
    {
        VertexElement* ve = ves[num];
        size_t offset = 0;
        for (int j=num-1; j>=0; --j)
        {
            offset += VertexElement::getVertexElementOffset(ves[j]->getVertexElementType());
        }
        glVertexAttribPointer(num, (int) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()), GL_FLOAT, GL_FALSE, (GLsizei)declare->getStride(), (void *) offset);
        glEnableVertexAttribArray(num);
    }
    
    op.mVertexData->markBinded();
}

void GLRenderSystem::updateProgramParameters(Pass* pass)
{
    pass->updateParameters(_sceneManager->getParameterDataSource());
}

void GLRenderSystem::setTextureUnitSettings(TextureUnitState* texState)
{
    const int unit = texState->getTextureUnit();
    const TexturePtr& texPtr = texState->getTexture();
    
    // Making sure the texture is loaded.
    bool ret = false;
    if (texState->getTextureAlias() == "")
    {
        ret = texState->_loadTexture();
    }
    else if (texState->getTextureAlias() == "Shadow")
    {
    }
    else if (texState->getTextureAlias() == "RTT")
    {
    }
    
    if (!ret)
    {
        // 纹理加载失败，直接返回。
        // FixYB: 是否考虑使用一个默认的纹理。。。？
        return;
    }
    
    GLenum target = texPtr->getTextureTarget();
    
    // Not mipmap.
    if (texState->getTextureFiltering(FT_MIP) == FO_NONE)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        
        _setTextureUnitFiltering(target, FT_MIN, texState->getTextureFiltering(FT_MIN));
        _setTextureUnitFiltering(target, FT_MAG, texState->getTextureFiltering(FT_MAG));
    }
    // Mipmap
    else
    {
        int numMipmaps = texState->getNumMipmaps();
        if (numMipmaps != 0)
        {
            // Set mipmap levels
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, numMipmaps);
        }
        // Set filters。
        _setTextureUnitFiltering(target, FT_MIP, texState->getTextureFiltering(FT_MIP));
        
        glGenerateMipmap(target);

    }
    
    // Wrapping.
    const UVWWrapMode& uvw = texState->getTextureWrapMode();
    _setTextureWrapMode(target, uvw);
    
    // Binding texture.
    _setTexture(unit, texPtr);
}

void GLRenderSystem::_setTexture(int unit, const TexturePtr& texPtr)
{
    activeTextureUnit(unit);
    glBindTexture(texPtr->getTextureTarget(), texPtr->getId());
}

void GLRenderSystem::_setTextureUnitFiltering(GLenum target, FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter)
{
    if (minFilter == FO_NEAR)
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    else
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    if (magFilter == FO_NEAR)
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    else
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    // Mipmap
    if (mipFilter == FO_NEAR)
    {
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    else if (mipFilter == FO_LINEAR)
    {
        glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

}

void GLRenderSystem::_setTextureUnitFiltering(GLenum target, FilterType type, FilterOptions opt)
{
    switch (type) {
        case FT_MIN:
        {
            if (opt == FO_NEAR)
                glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            else
                glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
            break;
            
        case FT_MAG:
        {
            if (opt == FO_NEAR)
                glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            else
                glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
            break;
            
        case FT_MIP:
        {
            if (opt == FO_NEAR)
            {
                glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            }
            else if (opt == FO_LINEAR)
            {
                glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            }
            
        }
            break;
    }
}
void GLRenderSystem::_setTextureWrapMode(GLenum target, UVWWrapMode wrapMode)
{
    glTexParameterf(target, GL_TEXTURE_WRAP_S, wrapMode.u);
    glTexParameterf(target, GL_TEXTURE_WRAP_T, wrapMode.v);
    glTexParameterf(target, GL_TEXTURE_WRAP_R, wrapMode.w);
}

void GLRenderSystem::activeTextureUnit(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
}




