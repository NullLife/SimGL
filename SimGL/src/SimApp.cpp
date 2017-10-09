//
//  App.cpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimApp.hpp"

App::App(const String &name, int width, int height)
{
    mWindow = new GameWindow(name, width, height);
}

App::~App()
{
    if (mWindow)
    {
        mWindow->registerKeyEvent(nullptr);
        mWindow->registerMouseEvent(nullptr);

        delete mWindow;
        mWindow = nullptr;
    }
}

void App::setSceneManager(SceneManager* sceneManager)
{
    mWindow->setSceneManager(sceneManager);
}

GameWindow *App::getGameWindow()
{
    return mWindow;
}
