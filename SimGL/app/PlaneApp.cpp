//
//  PlaneApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "PlaneApp.hpp"

PlaneApp::PlaneApp(const String& name, int width, int height) :
    App(name, width, height)
{
    mWindow->registerKeyEvent(this);
    mWindow->registerMouseEvent(this);
            
    // Scene manager
    mSceneManager = new SceneManager("Simple");
            
    // Set camera
    Camera* camera = mSceneManager->createCamera("DefualtCamera");
    camera->setPosition(Vec3(0, 3, 3));
    camera->setOrthWindow(width, height);
            
    setSceneManager(mSceneManager);
            
    initScene();
}

PlaneApp::~PlaneApp() {
    mWindow->registerKeyEvent(nullptr);
    mWindow->registerMouseEvent(nullptr);
    
    if (mWindow) {
        delete mWindow;
        mWindow = nullptr;
    }
}


void PlaneApp::initScene() {
    // plane
    SceneNode* root = mSceneManager->getRootNode();
    Model* planeModel = mSceneManager->createModel("plane.obj", "floor.material");
    root->attach(planeModel);
    const float scale = 0.5f;
    root->getTransform()->scaleX(scale);
    root->getTransform()->scaleY(scale);
    root->getTransform()->scaleZ(scale);
}

void PlaneApp::running() {
    mWindow->running();
}

void PlaneApp::keyCallback(int key, int scanCode, int action, int mods) {
    switch(action) {
        case GLFW_PRESS:
        case GLFW_REPEAT:
            if (key == GLFW_KEY_W)
            {
                mSceneManager->getCamera()->translate(0.0f, 0.0f, -1*0.5f);
            }
            else if (key == GLFW_KEY_S)
            {
                mSceneManager->getCamera()->translate(0.0f, 0.0f, 1*0.5f);
            }
            else if (key == GLFW_KEY_A)
            {
                mSceneManager->getCamera()->translate(-1*0.5f, 0.0f, 0.0f);
            }
            else if (key == GLFW_KEY_D)
            {
                mSceneManager->getCamera()->translate(1*0.5f, 0.0f, 0.0f);
            }
            else if (key == GLFW_KEY_Q)
            {
                mSceneManager->getCamera()->translate(0.0f, 1*0.5f, 0.0f);
            }
            else if (key == GLFW_KEY_Z)
            {
                mSceneManager->getCamera()->translate(0.0f, -1*0.5f, 0.0f);
            }
            break;
        default:break;
    }
}

void PlaneApp::mouseCallback(double x, double y) {
    
}
