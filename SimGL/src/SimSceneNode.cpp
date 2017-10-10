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
                                     ", model num: " + StringUtils::toString((int) mModels.size()));
    
    // Delete the models attached to node.
    ModelList::iterator iter = mModels.begin();
    for (; iter != mModels.end(); ++iter)
    {
        delete *iter;
    }
    mModels.clear();
    mModels.shrink_to_fit();
}

SceneManager* SceneNode::getSceneManager()
{
    return mSceneManager;
}

SceneNode::ModelList &SceneNode::getModelList()
{
    return mModels;
}

const unsigned int SceneNode::attach(Model *model)
{
    mModels.push_back(model);

    // Set parent
    model->setParent(this);

    return (unsigned int)(mModels.size()-1);
}

Model* SceneNode::getModel(const unsigned int index)
{
    return mModels[index];
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
    ModelList::iterator modelIter;
    for (modelIter=mModels.begin(); modelIter!=mModels.end(); ++modelIter)
    {
        (*modelIter)->updateRenderQueue(queue);
    }

    // Dealing its children recursively.
    NodeList::iterator childrenIter;
    for (childrenIter=mChildren.begin(); childrenIter!=mChildren.end(); ++childrenIter)
    {
        SceneNode *node = static_cast<SceneNode *>(*childrenIter);
        node->updateRenderQueue(queue);
    }
}

