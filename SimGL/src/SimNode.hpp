//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_NODE_HPP
#define SIMGL_NODE_HPP

#include "SimCommon.hpp"

class Transform;
class SceneManager;

class Node
{
public:
    typedef Vector<Node*> NodeList;

public:
    Node(const String& name);
    virtual ~Node();

protected:
    String mName;

    Node* mParent;
    NodeList mChildren;

    Transform* mTrans;

public:
    // Base
    const String& getName();

    void setParent(Node* parent);

    Node* getParent();
    
    const NodeList& getChildren();
    
    virtual Node* addChild(const String& childName);
    
    virtual Node* getChild(const String& childName);

    // Transform
    virtual Transform* getTransform();

protected:
    virtual Node* createChild(const String& childName) = 0;
    
    virtual void deleteNode(Node* node) = 0;
};


#endif //SIMGL_NODE_HPP
