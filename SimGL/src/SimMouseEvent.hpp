//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MOUSEEVENT_HPP
#define SIMGL_MOUSEEVENT_HPP


class MouseEvent {
public:
    virtual void mouseCallback(double x, double y) = 0;
};


#endif //SIMGL_MOUSEEVENT_HPP
