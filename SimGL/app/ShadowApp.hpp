//
//  ShadowApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/6/25.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef ShadowApp_hpp
#define ShadowApp_hpp

#include "SimApp.hpp"

class ShadowApp : public App
{
public:
    ShadowApp(const String& name, int width = 800, int height = 600);
    ~ShadowApp();
    
private:
    SceneManager* mSceneManager;
private:
    void initScene();
    
public:
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mouseCallback(double x, double y);
};

#endif /* ShadowApp_hpp */
