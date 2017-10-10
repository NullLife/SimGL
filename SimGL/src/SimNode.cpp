//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimNode.hpp"
#include "SimTransform.hpp"

Node::Node(const String &name) :
    mName(name),
    mParent(nullptr)
{
    mTrans = new Transform();
}

Node::~Node()
{
    LogManager::getSingleton().debug("delete Node: " + mName);
    
    mChildren.clear();
    mChildren.shrink_to_fit();
    
    delete mTrans;
    mTrans = nullptr;
}

const String &Node::getName()
{
    return mName;
}

void Node::setParent(Node *parent)
{
    mParent = parent;
}

Node *Node::getParent()
{
    return mParent;
}

Transform *Node::getTransform()
{
    return mTrans;
}

Node *Node::addChild(const String &childName)
{
    return createChild(childName);
}

Node *Node::getChild(const String &childName)
{
    return nullptr;
}

const Node::NodeList &Node::getChildren()
{
    return mChildren;
}
