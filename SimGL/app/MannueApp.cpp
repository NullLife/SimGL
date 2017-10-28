//
//  MannueApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "MannueApp.hpp"
#include "SimVertexArrayManager.hpp"

MannueApp::MannueApp(const String& name, int width, int height) :
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

MannueApp::~MannueApp()
{
    if (_sceneManager)
    {
        delete _sceneManager;
        _sceneManager = nullptr;
    }
}

void MannueApp::initScene()
{
    // plane
    SceneNode* root = _sceneManager->getRootNode();
    
    std::vector<Vec3> ps;
    std::vector<unsigned int> ids;
    
    ps.push_back(Vec3(-1, -1, 0));
    ps.push_back(Vec3(-0.9, -1, 0));
    ps.push_back(Vec3(2, 2, 0));
    ps.push_back(Vec3(1.9, 2, 0));
    
    ids.push_back(0);ids.push_back(1);ids.push_back(2);
    ids.push_back(0);ids.push_back(2);ids.push_back(3);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    VertexDataDeclare* vdd = new VertexDataDeclare();
    vdd->addElement(VertexElementSemantic::VES_POSITION, VertexElementType::VET_FLOAT3);
    VertexData* vd = new VertexData(vdd);
    HardwareVertexBuffer* vBuf = vd->createBuffer(vdd->getStride(), ps.size(), HardwareBuffer::Usage::HBU_STATIC);
    vBuf->writeData(&ps[0]);
    
    IndexData* idxd = new IndexData();
    HardwareIndexBuffer* iBuf = idxd->createBuffer(HardwareIndexBuffer::IndexType::IT_UInt, ids.size(), HardwareBuffer::Usage::HBU_STATIC);
    iBuf->writeData(&ids[0]);
    
    SimUInt64 key = vBuf->getBufferId();
    key += static_cast<SimUInt64>(iBuf->getBufferId()) << 32;
    VertexArrayManager::getSingleton().cacheVao(key, vao);
    
    Mesh* mesh = new Mesh("billboard");
    mesh->addSubMesh(vd, idxd);
    Model* mannue = new Model(mesh);
    mannue->setMaterial("billboard.material");
    mannue->setDrawType(DT_TRIANGLES);
    mannue->setPolygonMode(PM_FILL);
    
    mannue->getBoundingBox();

    root->attach(mannue);
}

void MannueApp::running()
{
    mWindow->running();
}

void MannueApp::keyCallback(int key, int scanCode, int action, int mods)
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

void MannueApp::mouseCallback(double x, double y)
{    
}
