//
//  MannueApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

/** Impletement Functions:
 * Custom mesh.
 */

#ifndef MannueApp_hpp
#define MannueApp_hpp

#include "SimApp.hpp"

class MannueApp : public App
{
public:
    MannueApp(const String& name, int width = 800, int height = 600);
    ~MannueApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mouseCallback(double x, double y);
    
private:
    void initScene();
    
    SceneManager* _sceneManager;
};


#endif /* MannueApp_hpp */
