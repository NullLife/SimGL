//
//  PlaneApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/3.
//  Copyright © 2017年 yangbin. All rights reserved.
//

/** Impletement Functions:
 * 1. Mipmap
 * 2. Geometry shader
 * 3. Billboard
 * 4. Particle System
 */

#ifndef PlaneApp_hpp
#define PlaneApp_hpp

#include "SimApp.hpp"

class PlaneApp : public App
{
public:
    PlaneApp(const String& name, int width = 800, int height = 600);
    ~PlaneApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mouseCallback(double x, double y);
    
private:
    void initScene();
    
    SceneManager* mSceneManager;
};


#endif /* PlaneApp_hpp */
