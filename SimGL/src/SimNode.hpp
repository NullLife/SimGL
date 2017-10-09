//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_NODE_HPP
#define SIMGL_NODE_HPP

#include "SimCommon.hpp"

class Transform;

class Node {
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

    // Transform
    virtual Transform* getTransform();

    // Struct operation
    virtual Node* addChild(const String& childName);

    virtual Node* getChild(const String& childName);

    const NodeList& getChildren();

    virtual void clear() = 0;

protected:
    virtual Node* createChild(const String& childName) = 0;

    virtual void deleteNode(Node* node) = 0;
};


#endif //SIMGL_NODE_HPP
