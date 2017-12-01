//
//  ManualApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

/** Impletement Functions:
 * Custom mesh.
 */

#ifndef ManualApp_hpp
#define ManualApp_hpp

#include "SimApp.hpp"

class ManualApp : public App
{
public:
    ManualApp(const String& name, int width = 800, int height = 600);
    ~ManualApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mousePositionCallback(double x, double y);
    
private:
    void initScene();
    
    SceneManager* _sceneManager;
};


#endif /* MannueApp_hpp */
