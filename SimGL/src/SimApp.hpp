//
//  SimApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include "SimResourceManager.hpp"
#include "SimMaterialManager.hpp"
#include "SimSceneManager.hpp"
#include "SimTransform.hpp"
#include "SimCamera.hpp"

#include "SimLight.hpp"

#include "SimGameWindow.hpp"
#include "SimKeyEvent.hpp"
#include "SimMouseEvent.hpp"

#include "SimVertexIndexData.hpp"
#include "SimModel.hpp"
#include "SimMesh.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"
#include "SimTexture.hpp"

class App : public KeyEvent, public MouseEvent {
public:
    App(const String& name, int width = 800, int height = 600);
    virtual ~App();

protected:
    GameWindow* mWindow;

public:
    virtual void keyCallback(int key, int scanCode, int action, int mods) = 0;

    virtual void mouseCallback(double x, double y) = 0;

    void setSceneManager(SceneManager* sceneManager);

    GameWindow* getGameWindow();
};

#endif /* App_hpp */
