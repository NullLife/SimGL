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
    _sceneManager = new SceneManager("Simple");
    setSceneManager(_sceneManager);
            
    // Set camera
    Camera* camera = _sceneManager->createCamera("DefualtCamera");
    camera->setPosition(Vec3(0, 1, 5));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->setOrthWindow(width, height);
    
    initScene();
}

PlaneApp::~PlaneApp()
{
    if (_sceneManager)
    {
        delete _sceneManager;
        _sceneManager = nullptr;
    }
}

void PlaneApp::initScene()
{
    // plane
    SceneNode* root = _sceneManager->getRootNode();
    Model* planeModel = _sceneManager->createModel("plane.obj", "floor.material");
    root->attach(planeModel);
    root->getTransform()->scale(Vec3(5.0f));
    
    // Billboard
    SceneNode* node = (SceneNode *) root->addChild("billboard");
    Model* billboardModel = _sceneManager->createModel("cube.obj", "billboard.material");
    
    billboardModel->setDrawType(DT_TRIANGLES);
    billboardModel->setPolygonMode(PM_FILL);
    
    billboardModel->getBoundingBox();

    node->attach(billboardModel);
    
//    node->getTransform()->pitch(glm::radians(45.0f));
    node->getTransform()->setPosition(Vec3(-1, 1, -1));
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
                _sceneManager->getCamera()->translate(0.0f, 0.0f, -step);
            else if (key == GLFW_KEY_S)
                _sceneManager->getCamera()->translate(0.0f, 0.0f, step);
            else if (key == GLFW_KEY_A)
                _sceneManager->getCamera()->translate(-step, 0.0f, 0.0f);
            else if (key == GLFW_KEY_D)
                _sceneManager->getCamera()->translate(step, 0.0f, 0.0f);
            else if (key == GLFW_KEY_Q)
                _sceneManager->getCamera()->translate(0.0f, step, 0.0f);
            else if (key == GLFW_KEY_Z)
                _sceneManager->getCamera()->translate(0.0f, -step, 0.0f);
            else if (key == GLFW_KEY_LEFT)
                _sceneManager->getCamera()->yaw(glm::radians(1.0f));
            else if (key == GLFW_KEY_RIGHT)
                _sceneManager->getCamera()->yaw(-glm::radians(1.0f));
            else if (key == GLFW_KEY_UP)
                _sceneManager->getCamera()->pitch(glm::radians(1.0f));
            else if (key == GLFW_KEY_DOWN)
                _sceneManager->getCamera()->pitch(-glm::radians(1.0f));
            
            else if (key == GLFW_KEY_N)
                _sceneManager->getRootNode()->getChild("billboard")->getTransform()->pitch(glm::radians(1.0f));
            else if (key == GLFW_KEY_J)
                _sceneManager->getRootNode()->getChild("billboard")->getTransform()->roll(glm::radians(1.0f));


            
        }
            break;
        default:break;
    }
}

void PlaneApp::mousePositionCallback(double x, double y)
{    
}
