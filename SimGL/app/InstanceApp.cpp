//
//  ParticleApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "InstanceApp.hpp"

#include "SimParticleSystem.hpp"
#include "SimInstanceBatch.hpp"
#include "SimInstancedModel.hpp"

#define NUM_INSTANCES 48

InstanceApp::InstanceApp(const String& name, int width, int height) :
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

InstanceApp::~InstanceApp()
{
    if (_sceneManager)
    {
        delete _sceneManager;
        _sceneManager = nullptr;
    }
}

void InstanceApp::initScene()
{
    // plane
    SceneNode* root = _sceneManager->getRootNode();
    Model* planeModel = _sceneManager->createModel("plane.obj", "floor.material");
    root->attach(planeModel);
    root->getTransform()->scale(Vec3(5.0f));
    
    SceneNode* node = (SceneNode *) root->addChild("Instances");
    InstanceBatch* cubeBatch = _sceneManager->createInstanceBatch("cube", "cube.obj", "cube_instances.material");
    cubeBatch->setNumInstancePerBatch(NUM_INSTANCES);
    cubeBatch->createAllInstances();
    
    
    // Construct a circle.
    float r = 10.0f;
    float pRad = 2 * M_PI / (float) NUM_INSTANCES;
    
    // Set positions.
    for (int i=0; i<NUM_INSTANCES; ++i)
    {
        InstancedModel* ins = cubeBatch->getInstances()[i];
        Transform* trans = ins->getTransform();
        float sita = pRad*i;
        LogManager::getSingleton().debug("rad", StringUtils::toString(sita));
        trans->setPosition(Vec3(r * cos(sita), 1, r * sin(sita)));
    }
    
    cubeBatch->build();
    
    node->attach(cubeBatch);
    node->getTransform()->scale(Vec3(0.2f));
}

void InstanceApp::running()
{
    mWindow->running();
}

void InstanceApp::keyCallback(int key, int scanCode, int action, int mods)
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

double lastX = 0, lastY = 0, disX = 0, disY = 0;
void InstanceApp::mousePositionCallback(double x, double y)
{
    if (x < 0 || y < 0)
        return;
    
    disX = x - lastX;
    disY = y - lastY;
    
    lastX = x;
    lastY = y;
}

void InstanceApp::mouseButtonCallback(int button, int action, int mods)
{
//    float factor = 0.5f;
//    switch(action)
//    {
//        case GLFW_PRESS:
//        case GLFW_REPEAT:
//        {
//            switch(button)
//            {
//                case GLFW_MOUSE_BUTTON_LEFT:
//
//                    break;
//                case GLFW_MOUSE_BUTTON_MIDDLE:
//
//                    break;
//                case GLFW_MOUSE_BUTTON_RIGHT:
//                    break;
//            }
//        }
//            break;
//        default:break;
//    }
}
