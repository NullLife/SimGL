//
// Created by yangbin on 2017/2/22.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimRenderQueue.hpp"
#include "SimRenderQueueGroup.hpp"
#include "SimRenderable.hpp"

RenderQueue::RenderQueue() {

}

RenderQueue::~RenderQueue() {
    LogManager::getSingleton().debug("delete RenderQueue");
    auto it = mQueueGroupMap.begin();
    while (it != mQueueGroupMap.end()) {
        delete it->second;
        ++it;
    }
    mQueueGroupMap.clear();
}

void RenderQueue::addRenderable(Renderable *renderable, RenderQueueGroupID groupID) {
    Technique * tech = renderable->getMaterial()->getBestTechnique();
    getRenderQueueGroup(groupID)->addRenderable(renderable, tech, 100);
}

RenderQueueGroup *RenderQueue::getRenderQueueGroup(RenderQueueGroupID groupID) {
    RenderQueueGroup *queueGroup = nullptr;
    auto it = mQueueGroupMap.find(groupID);
    if (it == mQueueGroupMap.end()) {
        queueGroup = new RenderQueueGroup(groupID);
        mQueueGroupMap.insert(RenderQueueGroupMap::value_type(groupID, queueGroup));
    } else {
        queueGroup = it->second;
    }
    return queueGroup;
}


RenderQueue::RenderQueueGroupMap &RenderQueue::getRenderQueueGroups() {
    return mQueueGroupMap;
}

RenderQueueGroup *RenderQueue::getDefaultRenderQueueGroup() {
    return getRenderQueueGroup(RQT_NORMAL);
}

void RenderQueue::clear() {
    auto it = mQueueGroupMap.begin();
    while (it != mQueueGroupMap.end()) {
        it->second->clear();
        ++it;
    }
    mQueueGroupMap.clear();
}

