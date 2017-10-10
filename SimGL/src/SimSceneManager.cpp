//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSceneManager.hpp"

#include "SimResourceManager.hpp"
#include "SimShaderManager.hpp"
#include "SimVertexArrayManager.hpp"
#include "SimGLProgramManager.hpp"

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
    mClearColor(Vec4(0.0f, 0.0f, 0.0f, 1.0f)),
    mClearDepth(1.0f),
    mSkyBoxNode(nullptr),
    mRootNode(nullptr),
    mCamera(nullptr),
    mBillboardMatrix(Mat4(1.0f)),
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
        mSkyBoxNode->deleteNode();
    }
    
    // Delete nodes
    if (mRootNode)
    {
        delete mRootNode;
        mRootNode = nullptr;
    }
    
    if (_paramDataSource)
    {
        delete _paramDataSource;
        _paramDataSource = nullptr;
    }
    
    // Delete vertex arrays.
    VertexArrayManager::destroySingleton();
    
    // Delete Shaders.
    ShaderManager::destroySingleton();
    
    // Delete programs.
    GLProgramManager::destroySingleton();
    
    // Delete Resources.
    ResourceManager::destroySingleton();

    // Delete Log.
    LogManager::destroySingleton();
}

const String &SceneManager::getName()
{
    return mName;
}

void SceneManager::setSkyBox(const String& mtl) {
    mSkyBoxNode = new SceneNode(this, "sky");
    
    Model* skyboxModel = createModel("cube.obj", mtl);
    mSkyBoxNode->attach(skyboxModel);
    
    // 100
    mSkyBoxNode->getTransform()->scale(Vec3(100.0f));
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

void SceneManager::deleteNode(SceneNode* node)
{
    node->deleteNode();
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

Model *SceneManager::createModel(Mesh* mesh)
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
static int t=0;
void SceneManager::_renderSingleObject(Renderable *rend, Pass* pass)
{
    // Vertex shader.
    GLShader* verShader = pass->getVertexShader();
    mRenderSystem->bindVertexShader(verShader);
    
    // Geometry shader.
    GLShader* geoShader = pass->getGeometryShader();
    mRenderSystem->bindGeometryShader(geoShader);
    
    // Fragment shader.
    GLShader* fragShader = pass->getFragmentShader();
    mRenderSystem->bindFragmentShader(fragShader);
    
    RenderOperation ro;
    ro.mSrcRend = rend;
    rend->getRenderOperation(ro);
    
    Mat4 modelMatrix = rend->getWorldTransforms();
    
    SubModel* subModel = dynamic_cast<SubModel*>(rend);
    glPolygonMode(GL_FRONT_AND_BACK, subModel->getParent()->getPolygonMode());
    
//    if (subModel && subModel->getParent()->getParentNode()->getName() == "billboard")
//    {
//        mCamera->setProjectionType(PT_ORTHOGRAPHIC);
//        _updateBillboard(subModel->getParent()->getParentNode(), mCamera);
//        modelMatrix *= mBillboardMatrix;
//    }
//    else
//    {
//        mCamera->setProjectionType(PT_PERSPECTIVE);
//    }
    
    _paramDataSource->setModelMatrix(modelMatrix);
    _paramDataSource->setCurrentPass(pass);
    _paramDataSource->setCurrentCamera(mCamera);
    _paramDataSource->setTime((float)glfwGetTime());
    
    if (mSkyBoxNode)
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        
    mRenderSystem->render(ro, pass);
}

static float oldAngle = 0;
void SceneManager::_updateBillboard(Node* billboard, Camera* camera)
{
    // Axis Aligned Billboards
    Transform* trans = billboard->getTransform();
    // The direction of the billboard facing.
    Vec3 facing = camera->getPosition() - trans->getPosition();
    facing = glm::normalize(facing);
    
    float tan = facing[0] / facing[2];
    float angle = atan(tan);
    float changed = angle - oldAngle;
    if (glm::abs(changed) > 0.001f)
    {
        mBillboardMatrix = glm::rotate(mBillboardMatrix, -changed, Z_AXIS);
    }
    oldAngle = angle;

//    // Then we will calculate the right vector.
//    Vec3 right = glm::cross(camera->getUp(), facing);
//    right = glm::normalize(right);
//
//    // Up.
//    Vec3 up = glm::cross(right, facing);
//    up = glm::normalize(up);
//
//    // Billboarding matrix.
//    mBillboardMatrix[0] = Vec4(right, 0);
//    mBillboardMatrix[1] = Vec4(up, 0);
//    mBillboardMatrix[2] = Vec4(facing, 0);
//    mBillboardMatrix[3] = Vec4(Vec3(0), 1);
}



