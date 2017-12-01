//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MOUSEEVENT_HPP
#define SIMGL_MOUSEEVENT_HPP


class MouseEvent {
public:
    virtual void mousePositionCallback(double x, double y) = 0;
    virtual void mouseButtonCallback(int button, int action, int mods) = 0;
};


#endif //SIMGL_MOUSEEVENT_HPP
