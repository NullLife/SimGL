//
//  SimMovableObject.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/9.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimMovableObject.hpp"
#include "SimSceneNode.hpp"
#include "SimRenderQueue.hpp"

MovableObject::MovableObject(const String& name) :
    _name(name),
    _sceneManager(nullptr),
    _parent(nullptr),
    _queueID(RQT_NORMAL),
    _visible(true)
{}

MovableObject::~MovableObject()
{}

const String& MovableObject::getName()
{
    return _name;
}

SceneManager* MovableObject::getSceneManager()
{
    return _sceneManager;
}

void MovableObject::setParent(Node* node)
{
    _parent = node;
}

Node* MovableObject::getParentNode()
{
    return _parent;
}

SceneNode* MovableObject::getParentSceneNode()
{
    return static_cast<SceneNode*>(_parent);
}

const BoundingBox& MovableObject::getBoundingBox()
{
    if (!_bb.valid())
        calcBoundingBox();
    
    return _bb;
}

void MovableObject::setRenderQueueGroup(const int queueID)
{
    _queueID = queueID;
}

const int MovableObject::getRenderQueueGroup()
{
    return _queueID;
}

void MovableObject::setVisible(bool visible)
{
    _visible = visible;
}

const bool MovableObject::getVisible()
{
    return _visible;
}
