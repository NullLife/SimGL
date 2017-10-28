//
//  ParticleApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "ParticleApp.hpp"

#include "SimParticleSystem.hpp"
#include "SimInstanceBatch.hpp"

#define NUM_INSTANCES 100

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
    camera->setPosition(Vec3(0, 1, -5));
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
    
//    for (unsigned int i=0; i<NUM_INSTANCES; ++i)
//    {
//
//    }
//    SceneNode* node = (SceneNode *) root->addChild("Instances");
//    InstanceBatch* cubeBatch = _sceneManager->createInstanceBatch("cube", "cube.obj", "cube_instances.material");
//    cubeBatch->setNumInstancePerBatch(NUM_INSTANCES);
//    node->attach(cubeBatch);
    
//    SceneNode* node = (SceneNode *) root->addChild("Particle");
//    ParticleSystem* ps = _sceneManager->createParticleSystem("Particle_TransFeedBack", 100);
//    ps->setMaterial("particle_tfb.material");
//    node->attach(ps);
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

void ParticleApp::mouseCallback(double x, double y)
{
}
