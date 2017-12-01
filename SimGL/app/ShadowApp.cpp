//
//  ShadowApp.cpp
//  SimGL
//
//  Created by yangbin on 2017/6/25.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "ShadowApp.hpp"

ShadowApp::ShadowApp(const String& name, int width, int height) :
        App(name, width, height) {
        
    mWindow->registerKeyEvent(this);
    mWindow->registerMouseEvent(this);
    
    // Scene manager
    mSceneManager = new SceneManager("Simple");
    // Open shadow
    mSceneManager->setEnableShadow(true);
    
    // Add lights
    Light* lt0 = new Light(LT_POINT_LIGHT);
    lt0->setPosition(Vec3(-2, 3, 1));
        
    mSceneManager->addLight(lt0);
    
    // Set camera
    Camera* camera = mSceneManager->createCamera("DefualtCamera");
    camera->setOrthWindow(width, height);
    camera->setPosition(Vec3(0, 6, 6));
    
    setSceneManager(mSceneManager);
        
        
    initScene();
}

ShadowApp::~ShadowApp() {
}


void ShadowApp::initScene() {
    
//    mSceneManager->setSkyBox("cube.obj",
//                  "bricks.jpg",
//                  "bricks.jpg",
//                  "bricks.jpg",
//                  "bricks.jpg",
//                  "bricks.jpg",
//                  "bricks.jpg");
    
//    SceneNode* root = mSceneManager->getRootNode();
//    Vector<Texture*> texArray;
//    texArray.resize(3);
//    texArray[0] = ResourceManager::getSingleton().load2DTexture("bricks.jpg");
//    texArray[1] = ResourceManager::getSingleton().load2DTexture("bricks_normal.jpg");
//    texArray[1]->setTag("NormalMap");
//    texArray[2] = ResourceManager::getSingleton().load2DTexture("bricks_disp.png");
//    texArray[2]->setTag("DispMap");
//    texArray[2]->setDispMapScaleAndOffset(0.03f, -0.5f);
//    Model* floor = mSceneManager->createModel("plane.obj");
//    for (SubModel* sub : floor->getSubModels()) {
//        Material* mtl = new Material("");
//        mtl->getTechnique()->addPass("shadow")->addTextures(&texArray[0], 3);
//        sub->setMaterial(mtl);
//    }
//    root->attach(floor);
//    const float scale = 10;
//    root->getTransform()->scaleX(scale);
//    root->getTransform()->scaleY(scale);
//    root->getTransform()->scaleZ(scale);
//    
//    texArray.clear();
    
//    SceneNode* planeNode = dynamic_cast<SceneNode*>(root->addChild("node_plane"));
//    assert(planeNode);
//    Texture* planeTex = ResourceManager::getSingleton().load2DTexture("bricks2.jpg");
//    Texture* planeNormalMapTex = ResourceManager::getSingleton().load2DTexture("bricks2_normal.jpg");
//    Texture* planeDispMapTex = ResourceManager::getSingleton().load2DTexture("bricks2_disp.jpg");
//    planeDispMapTex->setDispMapScaleAndOffset(0.04f, -1.0f);
//    planeNormalMapTex->setTag("NormalMap");
//    planeDispMapTex->setTag("DispMap");
//    texArray.resize(3);
//    texArray[0] = planeTex;
//    texArray[1] = planeNormalMapTex;
//    texArray[2] = planeDispMapTex;
//    Model* plane = mSceneManager->createModel("plane.obj");
//    for (SubModel* sub : plane->getSubModels()) {
//        Material* mtl = new Material("");
//        mtl->getTechnique()->addPass("shadow")->addTextures(&texArray[0], 3);
//        sub->setMaterial(mtl);
//    }
//    
//    planeNode->attach(plane);
//    planeNode->getTransform()->moveTo(Vec3(1.0, 1.0, -1.0));
//    //    planeNode->getTransform()->scaleX(0.5);
//    //    planeNode->getTransform()->scaleY(0.5);
//    //    planeNode->getTransform()->scaleZ(0.5);
//    
//    texArray.clear();
//    
//    SceneNode* cubeNode = dynamic_cast<SceneNode*>(root->addChild("node_cube"));
//    assert(cubeNode);
//    Texture* cubeTex = ResourceManager::getSingleton().load2DTexture("bricks2.jpg");
//    Texture* cubeNormalMapTex = ResourceManager::getSingleton().load2DTexture("bricks2_normal.jpg");
//    Texture* cubeDispMapTex = ResourceManager::getSingleton().load2DTexture("bricks2_disp.jpg");
//    cubeDispMapTex->setDispMapScaleAndOffset(0.04f, -1.0f);
//    cubeNormalMapTex->setTag("NormalMap");
//    cubeDispMapTex->setTag("DispMap");
//    
//    texArray.resize(3);
//    texArray[0] = cubeTex;
//    texArray[1] = cubeNormalMapTex;
//    texArray[2] = cubeDispMapTex;
//    Model* cube = mSceneManager->createModel("cube.obj");
//    for (SubModel* sub : cube->getSubModels()) {
//        Material* mtl = new Material("");
//        mtl->getTechnique()->addPass("shadow")->addTextures(&texArray[0], 3);
//        sub->setMaterial(mtl);
//    }
//    
//    cubeNode->attach(cube);
//    cubeNode->getTransform()->moveTo(Vec3(-1.0, 0.5, 1.0));
//    cubeNode->getTransform()->scaleX(0.5);
//    cubeNode->getTransform()->scaleY(0.5);
//    cubeNode->getTransform()->scaleZ(0.5);
//    
//    texArray.clear();
}

void ShadowApp::running() {
    mWindow->running();
}

void ShadowApp::keyCallback(int key, int scanCode, int action, int mods) {
}

void ShadowApp::mousePositionCallback(double x, double y) {
    
}
