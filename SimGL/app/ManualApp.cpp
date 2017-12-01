//
//  ManualApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "ManualApp.hpp"
#include "SimVertexArrayManager.hpp"

ManualApp::ManualApp(const String& name, int width, int height) :
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

ManualApp::~ManualApp()
{
    if (_sceneManager)
    {
        delete _sceneManager;
        _sceneManager = nullptr;
    }
}

void ManualApp::initScene()
{
    // plane
    SceneNode* root = _sceneManager->getRootNode();
    
    Vector<Vec3> ps;
    Vector<unsigned int> ids;
    
    // position
    ps.push_back(Vec3(-1, -1, 0));    //ps.push_back(Vec3(0, 0, 0));
    ps.push_back(Vec3(1, -1, 0));     //ps.push_back(Vec3(0, 1, 0));
    ps.push_back(Vec3(1, 1, 0));      //ps.push_back(Vec3(1, 1, 0));
    ps.push_back(Vec3(-1, 1, 0));     //ps.push_back(Vec3(1, 0, 0));
    // texcoords
    ps.push_back(Vec3(0, 0, 0));
    ps.push_back(Vec3(0, 1, 0));
    ps.push_back(Vec3(1, 1, 0));
    ps.push_back(Vec3(1, 0, 0));
    
    // index
    ids.push_back(0);ids.push_back(1);ids.push_back(2);
    ids.push_back(0);ids.push_back(2);ids.push_back(3);
    
    GLuint vao;
    SIM_CHECK_GL_ERROR( glGenVertexArrays(1, &vao) );
    glBindVertexArray(vao);
    
    VertexDataDeclare* vdd = new VertexDataDeclare(VEL_ALIGN);
    vdd->addElement(VertexElementSemantic::VES_POSITION, VertexElementType::VET_FLOAT3, 4);
    vdd->addElement(VertexElementSemantic::VES_TEXCOORD, VertexElementType::VET_FLOAT3, 4);
    
    VertexData* vd = new VertexData(vdd);
    vd->setNumberVertices(4);
    size_t bufferSize = ps.size() * sizeof(Vec3);
    HardwareVertexBuffer* vBuf = vd->createBuffer(bufferSize, HardwareBuffer::Usage::HBU_STATIC);
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

    root->attach(mannue);
}

void ManualApp::running()
{
    mWindow->running();
}

void ManualApp::keyCallback(int key, int scanCode, int action, int mods)
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

void ManualApp::mousePositionCallback(double x, double y)
{    
}
