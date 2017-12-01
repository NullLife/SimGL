//
//  ParticleApp.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/16.
//  Copyright © 2017年 yangbin. All rights reserved.
//

/** Impletement Functions: Image-based-render: IBR
 *  1. Particle System: Using Transform FeedBack.
 */

#ifndef ParticleApp_hpp
#define ParticleApp_hpp

#include "SimApp.hpp"

class ParticleApp : public App
{
public:
    ParticleApp(const String& name, int width = 800, int height = 600);
    ~ParticleApp();
    
    void running();
    void keyCallback(int key, int scanCode, int action, int mods);
    void mousePositionCallback(double x, double y);
    
private:
    void initScene();
    
    SceneManager* _sceneManager;
};

#endif /* ParticleApp_hpp */
