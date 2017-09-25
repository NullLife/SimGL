//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_KEYEVENT_HPP
#define SIMGL_KEYEVENT_HPP


class KeyEvent {
public:
    virtual void keyCallback(int key, int scanCode, int action, int mods) = 0;
};


#endif //SIMGL_KEYEVENT_HPP
