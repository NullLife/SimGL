//
//  InstanceApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

/** Impletement Functions:
 *  1. Instanceing render.
 */

#ifndef InstanceApp_hpp
#define InstanceApp_hpp

#include "SimApp.hpp"

class InstanceApp : public App
{
public:
    InstanceApp(const String& name, int width = 800, int height = 600);
    ~InstanceApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mousePositionCallback(double x, double y);
    void mouseButtonCallback(int button, int action, int mods);
    
private:
    void initScene();
    
    SceneManager* _sceneManager;
};

#endif /* InstanceApp_hpp */
