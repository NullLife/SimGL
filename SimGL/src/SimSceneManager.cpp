//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSceneManager.hpp"

#include "SimResourceManager.hpp"
#include "SimShaderManager.hpp"
#include "SimGLRenderSystem.hpp"
#include "SimRenderQueue.hpp"
#include "SimRenderQueueGroup.hpp"
#include "SimCamera.hpp"
#include "SimLight.hpp"
#include "SimModel.hpp"
#include "SimMesh.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"
#include "SimShader.hpp"
#include "SimFramebuffer.hpp"
#include "SimTexture.hpp"
#include "SimTransform.hpp"
#include "SimParameterDataSource.hpp"

SceneManager::SceneManager(const String &name) :
        mName(name),
        mClearColor(Vec4(0,0,0,1.0f)),
        mClearDepth(1.0f),
        mSkyBoxNode(nullptr),
        mRootNode(nullptr),
        mCamera(nullptr),
        mOpenShadow(false),
        mShadowFBuf(nullptr),
        mShadowPass(nullptr),
        mTestShadowPass(nullptr),
        mRenderSystem(new GLRenderSystem()),
        mRenderQueue(new RenderQueue())
{
    _paramDataSource = new ParameterDataSource();
    mRenderSystem->setSceneManager(this);
}

SceneManager::~SceneManager()
{
    LogManager::getSingleton().debug("delete SceneManager");
    
    // Delete camera
    if (mCamera)
    {
        delete mCamera;
        mCamera = nullptr;
    }
    // Delete lights
    for (auto iter=mLights.begin(); iter!= mLights.end(); ++iter)
    {
        delete *iter;
        mLights.erase(iter);
    }
    mLights.shrink_to_fit();
    
    // Delete shadow framebuffer
    if (mShadowFBuf)
    {
        delete mShadowFBuf;
        mShadowFBuf = nullptr;
    }
    if (mShadowPass)
    {
        delete mShadowPass;
        mShadowPass = nullptr;
    }
    if (mTestShadowPass)
    {
        delete mTestShadowPass;
        mTestShadowPass = nullptr;
    }
    // Delete shadow textures
    for (Texture * tex : mShadowTextureList)
    {
        delete tex;
    }
    mShadowTextureList.clear();
    mShadowTextureList.shrink_to_fit();
    
    // Delete Queues
    if (mRenderQueue)
    {
        delete mRenderQueue;
        mRenderQueue = nullptr;
    }
    
    // Delete SkyBox
    if (mSkyBoxNode)
    {
        delete mSkyBoxNode;
    }
    
    // Delete nodes
    if (mRootNode)
    {
        mRootNode->clear();
    
        auto nb = mNodeMap.begin();
        while (nb!=mNodeMap.end())
        {
            delete nb->second;
            ++nb;
        }
    }
    
    if (_paramDataSource)
    {
        delete _paramDataSource;
        _paramDataSource = nullptr;
    }
    
    //Delete Shaders
    ShaderManager::destroySingleton();
    
    // Delete Resource
    ResourceManager::destroySingleton();

    // Delete Log
    LogManager::destroySingleton();
}

const String &SceneManager::getName()
{
    return mName;
}

void SceneManager::setSkyBox(const String& imageName) {
    mSkyBoxNode = new SceneNode(this, "sky");

    
    const float scaleFactor = 10;
    mSkyBoxNode->getTransform()->scaleX(scaleFactor);
    mSkyBoxNode->getTransform()->scaleY(scaleFactor);
    mSkyBoxNode->getTransform()->scaleZ(scaleFactor);
    mSkyBoxNode->getTransform()->move(Vec3(0, scaleFactor-0.001, 0));
}

void SceneManager::setClearColor(const Vec4& color)
{
    mClearColor = color;
}

void SceneManager::setClearDepth(const float depth)
{
    mClearDepth = depth;
}

void SceneManager::setEnableShadow(const bool enableShadow)
{
    mOpenShadow = enableShadow;
}

const bool SceneManager::isEnableShadow() const
{
    return mOpenShadow;
}


void SceneManager::addLight(Light* light)
{
    mLights.push_back(light);
}

const LightList& SceneManager::getLights()
{
    return mLights;
}

Camera *SceneManager::createCamera(const String& name)
{
    mCamera = new Camera(name);
    return mCamera;
}

Camera *SceneManager::getCamera()
{
    return mCamera;
}

ParameterDataSource* SceneManager::getParameterDataSource()
{
    return _paramDataSource;
}

SceneNode *SceneManager::getRootNode()
{
    if (mRootNode == nullptr)
    {
        mRootNode = createNode("node_root");
        assert(mRootNode != nullptr);
    }
    return mRootNode;
}

SceneNode *SceneManager::createNode(const String &name)
{
    auto it = mNodeMap.find(name);
    if (it != mNodeMap.end())
    {
        LogManager::getSingleton().debug("SceneManager::createNode", "The node already existed.");
        return nullptr;
    }
    SceneNode *node = new SceneNode(this, name);
    
    mNodeMap.insert(NodeMap::value_type(name, node));
    return node;
}

Model *SceneManager::createModel(const String &name)
{
    Model* model = new Model(name);
    return model;
}

Model *SceneManager::createModel(const String &name, const String& material)
{
    Model* model = new Model(name);
    model->setMaterial(material);
    return model;
}

Model *SceneManager::createModel(MeshManager::MeshPtr mesh)
{
    return new Model(mesh);
}


void SceneManager::_processRenderQueue()
{
    // Dealing skybox
    if (mSkyBoxNode)
        mSkyBoxNode->updateRenderQueue(mRenderQueue);
    
    if (mRootNode)
        mRootNode->updateRenderQueue(mRenderQueue);
}

void SceneManager::_clearRenderQueue()
{
    //FixMe: error
    mRenderQueue->clear();
}

void SceneManager::updateScene()
{
    if (!mRootNode)
        return;

    _processRenderQueue();

    _render();

    _clearRenderQueue();
}


void SceneManager::_render()
{
    _renderScene();
}


void SceneManager::_renderScene()
{
    RenderQueue::RenderQueueGroupMap& groups = mRenderQueue->getRenderQueueGroups();
    RenderQueue::RenderQueueGroupMap::iterator iterGroups;
    
    RenderQueueGroup::PriorityMap::iterator iterPriorityGroups;
    QueuedRenderableCollection::PassGroupRenderableMap::iterator iterPass;
    QueuedRenderableCollection::RenderableList::iterator iterRenderable;
    
    for (iterGroups=groups.begin(); iterGroups!=groups.end(); ++iterGroups)
    {
        RenderQueueGroup* renderGroup = iterGroups->second;
        RenderQueueGroup::PriorityMap& priorityGroups = renderGroup->getRenderPriorityGroups();
        
        for (iterPriorityGroups=priorityGroups.begin(); iterPriorityGroups!=priorityGroups.end(); ++iterPriorityGroups)
        {
            RenderPriorityGroup* priorityGroup = iterPriorityGroups->second;
            QueuedRenderableCollection& solidCollection = priorityGroup->getSolidCollection();
            QueuedRenderableCollection::PassGroupRenderableMap& passGroupedMap = solidCollection.getPassGroupsRenderable();
            
            for (iterPass=passGroupedMap.begin(); iterPass!=passGroupedMap.end(); ++iterPass)
            {
                QueuedRenderableCollection::RenderableList* rendList = iterPass->second;

                for (iterRenderable=rendList->begin(); iterRenderable!=rendList->end(); ++iterRenderable)
                {
                    _renderSingleObject(*iterRenderable, iterPass->first);
                }
            }
        }
    }
}

void SceneManager::_renderSingleObject(Renderable *rend, Pass* pass)
{
    // Vertex shader
    GLShader* verShader = pass->getVertexShader();
    if (verShader)
        mRenderSystem->bindShader(verShader);
    
    // Fragment shader
    GLShader* fragShader = pass->getFragmentShader();
    if (fragShader)
        mRenderSystem->bindShader(fragShader);
    
    RenderOperation ro;
    ro.mSrcRend = rend;
    rend->getRenderOperation(ro);
    
    _paramDataSource->setModelMatrix(rend->getWorldTransforms());
    _paramDataSource->setCurrentPass(pass);
    _paramDataSource->setCurrentCamera(mCamera);
    
    mRenderSystem->render(ro, pass);
}




