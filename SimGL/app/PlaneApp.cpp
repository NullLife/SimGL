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
    root->attach(planeModel);
    root->getTransform()->scale(Vec3(5.0f));
    
    // Billboard
//    SceneNode* node = (SceneNode *) root->addChild("billboard");
//    Model* billboardModel = mSceneManager->createModel("cube.obj", "billboard.material");
//    billboardModel->setDrawType(DT_TRIANGLES);
//    billboardModel->setPolygonMode(Model::PolygonMode::PM_FILL);
//    node->attach(billboardModel);
//
//    node->getTransform()->setPosition(Vec3(0, 1, 0));
//    node->getTransform()->roll(1);
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
                mSceneManager->getCamera()->yaw(glm::radians(0.1f));
            else if (key == GLFW_KEY_RIGHT)
                mSceneManager->getCamera()->yaw(-glm::radians(0.1f));
        }
            break;
        default:break;
    }
}

void PlaneApp::mouseCallback(double x, double y)
{    
}
