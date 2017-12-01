//
//  ParticleApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "ParticleApp.hpp"

#include "SimParticleSystem.hpp"

#define NUM_INSTANCES 500

ParticleApp::ParticleApp(const String& name, int width, int height) :
    App(name, width, height)
{
    mWindow->registerKeyEvent(this);
    mWindow->registerMouseEvent(this);
    
    // Scene manager
    _sceneManager = new SceneManager("Simple");
    setSceneManager(_sceneManager);
    
    // Set camera
    Camera* camera = _sceneManager->createCamera("DefualtCamera");
    camera->setPosition(Vec3(0.0f, 5.0f, 10.0f));
    camera->setNearClipDistance(0.1f);
    camera->setFarClipDistance(1000.0f);
    camera->setOrthWindow(width, height);
    
    initScene();
}

ParticleApp::~ParticleApp()
{
    if (_sceneManager)
    {
        delete _sceneManager;
        _sceneManager = nullptr;
    }
}

void ParticleApp::initScene()
{
    // plane
    SceneNode* root = _sceneManager->getRootNode();
    Model* planeModel = _sceneManager->createModel("plane.obj", "floor.material");
    root->attach(planeModel);
    root->getTransform()->scale(Vec3(5.0f));
    
    SceneNode* child = root->createChild("grass");

    // Particles
    ParticleSystem* ps = _sceneManager->createParticleSystem("Particles", NUM_INSTANCES);
    ps->setMaterial("particle_billboard.material");
    child->attach(ps);
}

void ParticleApp::running()
{
    mWindow->running();
}

void ParticleApp::keyCallback(int key, int scanCode, int action, int mods)
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
            
        }
            break;
        default:break;
    }
}

void ParticleApp::mousePositionCallback(double x, double y)
{
    //LogManager::getSingleton().debug("mouseCallback", StringUtils::toString(Vec3(x, y, 0)));
}
