//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimSceneNode.hpp"
#include "SimSceneManager.hpp"
#include "SimRenderQueueGroup.hpp"
#include "SimModel.hpp"

SceneNode::SceneNode(SceneManager* sm, const String &name) :
        Node(name), mSceneManager(sm) {

}

SceneNode::~SceneNode() {
    LogManager::getSingleton().debug("delete SceneNode");
}

SceneManager* SceneNode::getSceneManager() {
    return mSceneManager;
}

SceneNode::ModelList &SceneNode::getModelList() {
    return mModels;
}

const unsigned int SceneNode::attach(Model *model) {
    mModels.push_back(model);

    // Set parent
    model->setParent(this);

    return (unsigned int)(mModels.size()-1);
}

Model* SceneNode::getModel(const unsigned int index) {
    return mModels[index];
}

void SceneNode::clear() {
    // Clear models which were attached in this node.
    for (Model* m : mModels) {
        delete m;
    }
    mModels.clear();
    mModels.shrink_to_fit();

    for (Node* n: mChildren) {
        n->clear();
    }
}

SceneNode *SceneNode::createChild(const String &childName) {
    SceneNode* node = mSceneManager->createNode(childName);
    if (node) {
        node->setParent(this);
        mChildren.push_back(node);
    }
    return node;
}

void SceneNode::deleteNode(Node *node) {
//    for (Node* n: mChildren) {
//        SceneNode* sn = static_cast<SceneNode*>(n);
//        deleteNode(sn);
//
//    }
//    node->mChildren.clear();
//    node->mChildren.shrink_to_fit();
//    delete sn;
//    sn = nullptr;
}

void SceneNode::updateRenderQueue(RenderQueue *queue) {
    // Find visible

    // Add models in this node
    ModelList::iterator modelIter;
    for (modelIter=mModels.begin(); modelIter!=mModels.end(); ++modelIter) {
        (*modelIter)->updateRenderQueue(queue);
    }

    // Dealing its children recursively.
    NodeList::iterator childrenIter;
    for (childrenIter=mChildren.begin(); childrenIter!=mChildren.end(); ++childrenIter) {
        SceneNode *node = static_cast<SceneNode *>(*childrenIter);
        node->updateRenderQueue(queue);
    }
}

