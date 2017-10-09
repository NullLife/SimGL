//
//  SkyBoxApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/9/26.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SkyBoxApp_hpp
#define SkyBoxApp_hpp

#include "SimApp.hpp"

class SkyBoxApp : public App
{
public:
    SkyBoxApp(const String& name, int width = 800, int height = 600);
    ~SkyBoxApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mouseCallback(double x, double y);
    
private:
    void initScene();
    
    SceneManager* _sceneManager;

};

#endif /* SkyBoxApp_hpp */
