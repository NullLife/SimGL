//
//  SkyBoxApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/9/26.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SkyBoxApp.hpp"

SkyBoxApp::SkyBoxApp(const String& name, int width, int height) :
    App(name, width, height)
{
    mWindow->registerKeyEvent(this);
    mWindow->registerMouseEvent(this);
    
    // Scene manager
    _sceneManager = new SceneManager("Simple");
    
    // Set camera
    Camera* camera = _sceneManager->createCamera("DefualtCamera");
    camera->setPosition(Vec3(0, 1, 5));
    camera->setOrthWindow(width, height);
    
    setSceneManager(_sceneManager);
    
    initScene();
}

SkyBoxApp::~SkyBoxApp()
{
}

void SkyBoxApp::initScene()
{
    // plane
    _sceneManager->setSkyBox("skybox.material");
//    SceneNode* root = _sceneManager->getRootNode();
}

void SkyBoxApp::running() {
    mWindow->running();
}

void SkyBoxApp::keyCallback(int key, int scanCode, int action, int mods)
{
    float step = 0.5f;
    switch(action)
    {
        case GLFW_PRESS:
        case GLFW_REPEAT:
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
            break;
        default:break;
    }
}

void SkyBoxApp::mouseCallback(double x, double y)
{
}
