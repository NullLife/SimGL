//
//  SimGameWindow.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef GameWindow_hpp
#define GameWindow_hpp

#include "SimCommon.hpp"

class KeyEvent;

class MouseEvent;

class SceneManager;

class GameWindow {
public:
    GameWindow(const String &name, const int &width = 800, const int &height = 600);

    ~GameWindow();

private:
    GLFWwindow *mWindow;
    String mName;
    int        mWidth;
    int        mHeight;
    int        mBufferWidth;
    int        mBufferHeight;

    Vec4 mClearColor;
    float      mClearDepth;

    double mDeltaTime;

    // event
    KeyEvent   *mKeyEvent;
    MouseEvent *mMouseEvent;

    SceneManager* mSceneManager;

public:
    void setClearColor(const Vec4 &color);

    const Vec4 &getClearColor();

    void setClearDepth(const float &depth);

    const float &getClearDepth();

    const int &getWidth();

    const int &getHeight();

    const int &getFramebufferWidth();

    const int &getFramebufferHeight();


    void registerKeyEvent(KeyEvent *event);

    KeyEvent *getKeyEvent();

    void registerMouseEvent(MouseEvent *event);

    MouseEvent *getMouseEvent();

    void setSceneManager(SceneManager * sceneManager);

    SceneManager* getSceneManager();

    void running();

private:
    void initSystem();

    void processInput();

    void drawScene();
};

#endif /* GameWindow_hpp */
