//
//  GameWindow.cpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimGameWindow.hpp"
#include "SimKeyEvent.hpp"
#include "SimMouseEvent.hpp"
#include "SimSceneManager.hpp"

static void keyCallback(GLFWwindow *window,
                        int key,
                        int scanCode,
                        int action,
                        int mods) {
    GameWindow *game  = reinterpret_cast<GameWindow *>(glfwGetWindowUserPointer(window));
    KeyEvent   *event = game->getKeyEvent();
    if (event) {
        event->keyCallback(key, scanCode, action, mods);
    }
}

static void mouseCallback(GLFWwindow *window,
                              double x, double y) {
    GameWindow *game  = reinterpret_cast<GameWindow *>(glfwGetWindowUserPointer(window));
    MouseEvent *event = game->getMouseEvent();
    if (event) {
        event->mouseCallback(x, y);
    }
}


GameWindow::GameWindow(const String &name, const int &width, const int &height) :
        mWindow(nullptr), mName(name),
        mWidth(width), mHeight(height),
        mBufferWidth(0), mBufferHeight(0),
        mKeyEvent(nullptr), mMouseEvent(nullptr),
        mClearColor(Vec4(0.0)), mClearDepth(1.0), mDeltaTime(0) {

    initSystem();
}

GameWindow::~GameWindow() {
    LogManager::getSingleton().debug("delete GameWindow");
    if (mSceneManager) {
        delete mSceneManager;
        mSceneManager = nullptr;
    }
}

void GameWindow::setClearColor(const Vec4 &color) {
    mClearColor = color;
    
    if (mSceneManager) {
        
    }
}

const Vec4 &GameWindow::getClearColor() {
    return mClearColor;
}

void GameWindow::setClearDepth(const float &depth) {
    mClearDepth = depth;
    
    if (mSceneManager) {
        
    }
}

const float &GameWindow::getClearDepth() {
    return mClearDepth;
}

const int &GameWindow::getWidth() {
    return mWidth;
}

const int &GameWindow::getHeight() {
    return mHeight;
}

const int &GameWindow::getFramebufferWidth() {
    return mBufferWidth;
}

const int &GameWindow::getFramebufferHeight() {
    return mBufferHeight;
}

void GameWindow::registerKeyEvent(KeyEvent *event) {
    mKeyEvent = event;
}

KeyEvent *GameWindow::getKeyEvent() {
    return mKeyEvent;
}

void GameWindow::registerMouseEvent(MouseEvent *event) {
    mMouseEvent = event;
}

MouseEvent *GameWindow::getMouseEvent() {
    return mMouseEvent;
}

void GameWindow::setSceneManager(SceneManager * sceneManager) {
    mSceneManager = sceneManager;
}

SceneManager* GameWindow::getSceneManager() {
    return mSceneManager;
}

void GameWindow::initSystem() {
    // Initialize GLFW
    glfwInit();
    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Mac need it
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwSwapInterval(1);

    // Create window
    mWindow = glfwCreateWindow(mWidth, mHeight, mName.c_str(), NULL, NULL);
    if (!mWindow) {
        LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "Failed to create window");
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(mWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "Failed to initialize GLEW");
    }

    if (glewIsSupported("GL_VERSION_4_1")) {
        LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "Ready for OpenGL 4.1");
    } else {
        LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "OpenGL 4.1 not supported");
    }

    // Share window's pointer
    glfwSetWindowUserPointer(mWindow, (void *) this);

    // Get version info
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version  = glGetString(GL_VERSION);
    LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "Renderer:" + std::to_string(*renderer));
    LogManager::getSingletonPtr()->debug("GameWindow#initSystem", "OpenGL version supported:" + std::to_string(*version));

    glfwGetFramebufferSize(mWindow, &mBufferWidth, &mBufferHeight);
    glViewport(0, 0, mBufferWidth, mBufferHeight);
}

void GameWindow::running() {

    processInput();

    while (!glfwWindowShouldClose(mWindow)) {
        double currentTime = glfwGetTime();

        // poll for and process events
        glfwPollEvents();

        // render
        drawScene();

        glfwSwapBuffers(mWindow);

        mDeltaTime = glfwGetTime() - currentTime;
    }
}

void GameWindow::processInput() {
    glfwSetKeyCallback(mWindow, keyCallback);
    glfwSetCursorPosCallback(mWindow, mouseCallback);
}

void GameWindow::drawScene() {
    glClearBufferfv(GL_COLOR, 0, &mClearColor[0]);
    glClearBufferfv(GL_DEPTH, 0, &mClearDepth);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);

    // update
    if (!mSceneManager) {
        return;
    }

    mSceneManager->updateScene();
}


