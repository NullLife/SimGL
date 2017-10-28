//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SCENENODE_HPP
#define SIMGL_SCENENODE_HPP

#include "SimNode.hpp"

class SceneManager;
class RenderQueue;
class MovableObject;
class Mesh;

class SceneNode : public Node
{
public:
    typedef Vector<MovableObject*> MovableObjectList;

public:
    SceneNode(SceneManager* sm, const String& name);
    virtual ~SceneNode();

private:
    MovableObjectList _movableObjList;
    SceneManager* mSceneManager;

public:
    SceneManager* getSceneManager();
    
    MovableObjectList& getMovableObjectList() { return _movableObjList; }

    const unsigned int attach(MovableObject* object);

    MovableObject* getMovableObject(const unsigned int index);

    void updateRenderQueue(RenderQueue *queue);
    
    void deleteNode();
    
    void deleteNode(Node* node);

    SceneNode* createChild(const String& childName);
    
    virtual SceneNode* getChild(const String& childName);
};

#endif //SIMGL_SCENENODE_HPP
