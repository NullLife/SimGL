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
    mName(name),
    mManager(nullptr),
    mParent(nullptr),
    mQueueID(RQT_NORMAL),
    mVisible(true)
{}

MovableObject::~MovableObject()
{}

const String& MovableObject::getName()
{
    return mName;
}

SceneManager* MovableObject::getSceneManager()
{
    return mManager;
}

Node* MovableObject::getParentNode()
{
    return mParent;
}

SceneNode* MovableObject::getParentSceneNode()
{
    return static_cast<SceneNode*>(mParent);
}

const BoundingBox& MovableObject::getBoundingBox()
{
    if (!_bb.valid())
        calcBoundingBox();
    
    return _bb;
}

void MovableObject::setRenderQueueGroup(const int queueID)
{
    mQueueID = queueID;
}

const int MovableObject::getRenderQueueGroup()
{
    return mQueueID;
}

void MovableObject::setVisible(bool visible)
{
    mVisible = visible;
}

const bool MovableObject::getVisible()
{
    return mVisible;
}
