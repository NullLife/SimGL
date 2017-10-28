//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SCENEMANAGER_HPP
#define SIMGL_SCENEMANAGER_HPP

#include "SimCommon.hpp"
#include "SimMeshManager.hpp"
#include "SimSceneNode.hpp"
#include "SimTexture.hpp"
#include "SimLight.hpp"

class GLRenderSystem;
class RenderQueue;
class Renderable;
class Model;
class SubModel;
class Mesh;
class Pass;
class Material;
class Camera;
class Framebuffer;
class ParameterDataSource;

class ParticleSystem;
class InstanceBatch;

typedef HashMap<String, Node *> NodeMap;
typedef Vector<Texture*> ShadowTextureList;

class SceneManager
{
public:
    friend class SceneNode;
    
public:
    SceneManager(const String &name);
    ~SceneManager();
    
    const String &getName();
    
    void setSkyBox(const String& imageName);
    
    void setClearColor(const Vec4& color);
    void setClearDepth(const float depth);
    
    void setEnableShadow(const bool enableShadow);
    const bool isEnableShadow() const;
    
    void addLight(Light* light);
    const LightList& getLights();

    Camera *createCamera(const String& name);
    Camera *getCamera();
    
    ParameterDataSource* getParameterDataSource();

    SceneNode *getRootNode();

    SceneNode *createNode(const String &name);
    SceneNode *getNode(const String &name);
    void deleteNode(SceneNode* node);
    
    Model *createModel(const String &name);
    Model *createModel(const String &name, const String& material);
    Model *createModel(Mesh* mesh);
    
    InstanceBatch* createInstanceBatch(const String& name, const String& mesh, const String& material);
    
    ParticleSystem* createParticleSystem(const String& name, size_t poolSize);

    void updateScene();

private:
    void _processRenderQueue();
    void _render();
    void _clearRenderQueue();
    void _renderScene();
    void _renderSingleObject(Renderable *rend, Pass* pass);
    
    void _renderBox(Renderable *rend);
    
    void _updateBillboard(Node* billboard, Camera* camera);
    
    String mName;
    
    SceneNode* mSkyBoxNode;
    SceneNode *mRootNode;
    Mat4 mBillboardMatrix;
    
    NodeMap mNodeMap;
    
    GLRenderSystem *mRenderSystem;
    RenderQueue *mRenderQueue;
    
    Camera *mCamera;
    
    LightList mLights;
    
    bool mOpenShadow;
    ShadowTextureList mShadowTextureList;
    Framebuffer* mShadowFBuf;
    Pass* mShadowPass;
    Pass* mTestShadowPass;
    
    Vec4 mClearColor;
    float mClearDepth;
    
    ParameterDataSource* _paramDataSource;
};


#endif //SIMGL_SCENEMANAGER_HPP
