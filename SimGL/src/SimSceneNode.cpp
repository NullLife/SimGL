//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSceneNode.hpp"

#include "SimRenderQueueGroup.hpp"
#include "SimModel.hpp"

SceneNode::SceneNode(SceneManager* sm, const String &name) :
    Node(name),
    mSceneManager(sm)
{
}

SceneNode::~SceneNode()
{
    LogManager::getSingleton().debug("delete SceneNode: " + mName +
                                     ", child node num: " + StringUtils::toString((int)mChildren.size()) +
                                     ", model num: " + StringUtils::toString((int) _movableObjList.size()));
    
    // Delete the models attached to node.
    MovableObjectList::iterator iter = _movableObjList.begin();
    for (; iter != _movableObjList.end(); ++iter)
    {
        delete *iter;
    }
    _movableObjList.clear();
    _movableObjList.shrink_to_fit();
}

SceneManager* SceneNode::getSceneManager()
{
    return mSceneManager;
}

const unsigned int SceneNode::attach(MovableObject *object)
{
    object->setParent(this);
    _movableObjList.push_back(object);
    return (unsigned int)(_movableObjList.size()-1);
}

MovableObject* SceneNode::getMovableObject(const unsigned int index)
{
    return _movableObjList[index];
}

SceneNode *SceneNode::createChild(const String &childName)
{
    SceneNode* node = mSceneManager->createNode(childName);
    if (node)
    {
        node->setParent(this);
        mChildren.push_back(node);
    }
    return node;
}

SceneNode* SceneNode::getChild(const String& childName)
{
    return mSceneManager->getNode(childName);
}

void SceneNode::deleteNode()
{
    deleteNode(this);
}

void SceneNode::deleteNode(Node *node)
{
    // Delete all children.
    NodeList::iterator iter = mChildren.begin();
    NodeList::const_iterator endIter = mChildren.end();
    while (iter != endIter)
    {
        deleteNode(*iter);
        ++iter;
    }
    
    // Remove the node in the global map.
    mSceneManager->mNodeMap.erase(node->getName());
    
    delete node;
    node = nullptr;
}

void SceneNode::updateRenderQueue(RenderQueue *queue)
{
    // Find visible

    // Add models in this node
    MovableObjectList::iterator modelIter = _movableObjList.begin();
    for (; modelIter!=_movableObjList.end(); ++modelIter)
    {
        (*modelIter)->updateRenderQueue(queue);
    }

    // Dealing its children recursively.
    NodeList::iterator childrenIter = mChildren.begin();
    for (; childrenIter!=mChildren.end(); ++childrenIter)
    {
        SceneNode *node = static_cast<SceneNode *>(*childrenIter);
        node->updateRenderQueue(queue);
    }
}

