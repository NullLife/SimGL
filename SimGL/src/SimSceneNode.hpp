//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SCENENODE_HPP
#define SIMGL_SCENENODE_HPP

#include "SimNode.hpp"

class SceneManager;
class RenderQueue;
class Model;
class Mesh;

class SceneNode : public Node
{
public:
    typedef Vector<Model*> ModelList;

public:
    SceneNode(SceneManager* sm, const String& name);
    virtual ~SceneNode();

private:
    ModelList mModels;
    SceneManager* mSceneManager;

public:
    SceneManager* getSceneManager();
    
    ModelList& getModelList();

    const unsigned int attach(Model* model);

    Model* getModel(const unsigned int index);

    void updateRenderQueue(RenderQueue *queue);
    
    void deleteNode();
    
    void deleteNode(Node* node);

    SceneNode* createChild(const String& childName);
};

#endif //SIMGL_SCENENODE_HPP
