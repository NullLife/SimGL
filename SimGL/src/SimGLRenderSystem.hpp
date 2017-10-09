//
// Created by yangbin on 2017/3/4.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_GLRENDERSYSTEM_HPP
#define SIMGL_GLRENDERSYSTEM_HPP

#include "SimCommon.hpp"
#include "SimTextureUnitState.hpp"

class SceneManager;
class Pass;
class GLSLShader;
class RenderOperation;

class GLRenderSystem {
public:
    GLRenderSystem();
    ~GLRenderSystem();

public:
    void setSceneManager(SceneManager* sm);
    
    void bindVertexShader(GLShader* verShader);
    void bindGeometryShader(GLShader* geoShader);
    void bindFragmentShader(GLShader* fragShader);
    
    void updateProgramParameters(Pass* pass);
    void setTextureUnitSettings(TextureUnitState* texState);
    
    void render(RenderOperation &op, Pass* pass);
    
private:
    void _commitVertexData(RenderOperation &op);
    
    // Set texture.
    void _setTexture(int unit, const TexturePtr& texPtr);
    void _setTextureUnitFiltering(GLenum target, FilterOptions minFilter, FilterOptions magFilter, FilterOptions mipFilter);
    void _setTextureUnitFiltering(GLenum target, FilterType type, FilterOptions opt);
    void _setTextureWrapMode(GLenum target, UVWWrapMode wrapMode);
    void activeTextureUnit(int unit);

    GLShader *_currVertShader;
    GLShader *_currGeoShader;
    GLShader *_currFragShader;
    
    SceneManager* _sceneManager;
};


#endif //SIMGL_GLRENDERSYSTEM_HPP
