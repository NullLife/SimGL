//
//  PlaneApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "PlaneApp.hpp"
#include "SimVertexArrayManager.hpp"

PlaneApp::PlaneApp(const String& name, int width, int height) :
    App(name, width, height)
{
    mWindow->registerKeyEvent(this);
    mWindow->registerMouseEvent(this);
            
    // Scene manager
    mSceneManager = new SceneManager("Simple");
    setSceneManager(mSceneManager);
            
    // Set camera
    Camera* camera = mSceneManager->createCamera("DefualtCamera");
    camera->setPosition(Vec3(0, 1, 5));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->setOrthWindow(width, height);
    
    initScene();
}

PlaneApp::~PlaneApp()
{
    if (mSceneManager)
    {
        delete mSceneManager;
        mSceneManager = nullptr;
    }
}

void PlaneApp::initScene()
{
    // plane
    SceneNode* root = mSceneManager->getRootNode();
    Model* planeModel = mSceneManager->createModel("plane.obj", "floor.material");
//    planeModel->setPolygonMode(Model::PolygonMode::PM_LINE);
    root->attach(planeModel);
    root->getTransform()->scale(Vec3(5.0f));
    
    // Billboard
    SceneNode* node = (SceneNode *) root->addChild("billboard");
    VertexDataDeclare* vdd = new VertexDataDeclare();
    vdd->addElement(new VertexElement(VertexElementSemantic::VES_POSITION, VertexElementType::VET_FLOAT3));
    vdd->addElement(new VertexElement(VertexElementSemantic::VES_DIFFUSECOLOR, VertexElementType::VET_FLOAT4));

    VertexData* verD = new VertexData(vdd);
    // Create vertex array object.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    verD->initBuffer(vdd->getStride(), 3, HardwareBuffer::Usage::HBU_STATIC);
    float verticeData[] = {
        0, 1, 0,   1, 0, 0, 1,
        -1, 0, 0,  1, 0, 0, 1,
        1, 0, 0,   1, 0, 0, 1
    };
    verD->getBuffer()->writeData(0, verD->getBuffer()->getSize(), verticeData);

    IndexData* idxD = new IndexData();
    idxD->initBuffer(HardwareIndexBuffer::IndexType::IT_UInt, 3, HardwareBuffer::Usage::HBU_STATIC);
    unsigned int indexData[] = {
        0, 1, 2
    };
    idxD->getBuffer()->writeData(0, idxD->getBuffer()->getSize(), indexData);

    Mesh* mesh = new Mesh("billboard");
    mesh->addSubMesh(verD, idxD);
    Model* billboardModel = mSceneManager->createModel(mesh);
    billboardModel->setMaterial("billboard.material");
    billboardModel->setDrawType(DT_TRIANGLES);
//    billboardModel->setPolygonMode(Model::PolygonMode::PM_LINE);
    node->attach(billboardModel);

    node->getTransform()->setPosition(Vec3(0, 1, 0));

    SimUInt64 vaoKey = verD->getBuffer()->getBufferId();
    vaoKey += (SimUInt64)(idxD->getBuffer()->getBufferId()) << 32;
    VertexArrayManager::getSingleton().cacheVao(vaoKey, vao);
}

void PlaneApp::running()
{
    mWindow->running();
}

void PlaneApp::keyCallback(int key, int scanCode, int action, int mods)
{
    float step = 0.5f;
    switch(action)
    {
        case GLFW_PRESS:
        case GLFW_REPEAT:
        {
            if (key == GLFW_KEY_W)
                mSceneManager->getCamera()->translate(0.0f, 0.0f, -step);
            else if (key == GLFW_KEY_S)
                mSceneManager->getCamera()->translate(0.0f, 0.0f, step);
            else if (key == GLFW_KEY_A)
                mSceneManager->getCamera()->translate(-step, 0.0f, 0.0f);
            else if (key == GLFW_KEY_D)
                mSceneManager->getCamera()->translate(step, 0.0f, 0.0f);
            else if (key == GLFW_KEY_Q)
                mSceneManager->getCamera()->translate(0.0f, step, 0.0f);
            else if (key == GLFW_KEY_Z)
                mSceneManager->getCamera()->translate(0.0f, -step, 0.0f);
            else if (key == GLFW_KEY_LEFT)
                mSceneManager->getCamera()->yaw(0.01f);
            else if (key == GLFW_KEY_RIGHT)
                mSceneManager->getCamera()->yaw(-0.01f);
        }
            break;
        default:break;
    }
}

void PlaneApp::mouseCallback(double x, double y)
{    
}
