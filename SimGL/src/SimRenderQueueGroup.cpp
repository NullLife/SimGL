//
// Created by yangbin on 2017/2/22.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimRenderQueueGroup.hpp"
#include "SimModel.hpp"

//---------------------------RenderQueueGroup------------------------------------//

RenderQueueGroup::RenderQueueGroup(RenderQueueGroupID groupID) :
    mGroupID(groupID) {

}

RenderQueueGroup::~RenderQueueGroup() {
    PriorityMap::iterator iter;
    for (iter=mPriorityGroups.begin(); iter!=  mPriorityGroups.end(); ++iter) {
        // Clear RenderPriorityGroup
        iter->second->clear();
        // Delete RenderPriorityGroup
        delete (iter->second);
    }
    mPriorityGroups.clear();
    LogManager::getSingleton().debug("Delete RenderQueueGroup groupID", std::to_string(mGroupID));
}

const RenderQueueGroupID& RenderQueueGroup::getGroupID() const {
    return mGroupID;
}

void RenderQueueGroup::addRenderable(Renderable *renderable, Technique* tech, short priority) {
    RenderPriorityGroup* group;
    PriorityMap::iterator i = mPriorityGroups.find(priority);
    if (i == mPriorityGroups.end()) {
        group = new RenderPriorityGroup(this);
        mPriorityGroups.insert(PriorityMap::value_type(priority, group));
    } else {
        group = i->second;
    }
    group->addSolidRenderable(renderable, tech);
}

RenderQueueGroup::PriorityMap& RenderQueueGroup::getRenderPriorityGroups() {
    return mPriorityGroups;
}


void RenderQueueGroup::clear() {
    PriorityMap::iterator iter;
    for (iter=mPriorityGroups.begin(); iter!=  mPriorityGroups.end(); ++iter) {
        // Clear RenderableList
        iter->second->clear();
    }
    mPriorityGroups.clear();
}


//-------------------------RenderPriorityGroup------------------------------//

RenderPriorityGroup::RenderPriorityGroup(RenderQueueGroup* parent) :
        mParent(parent) {
    
}

RenderPriorityGroup::~RenderPriorityGroup() {
    clear();
}

void RenderPriorityGroup::addSolidRenderable(Renderable *renderable, Technique* tech) {
    const Technique::PassList& passes = tech->getPassList();
    for (Pass* pass : passes) {
        mSoildCollection.addRenderable(pass, renderable);
    }
}

QueuedRenderableCollection& RenderPriorityGroup::getSolidCollection() {
    return mSoildCollection;
}

void RenderPriorityGroup::clear() {
    mSoildCollection.clear();
}


//-------------------------QueuedRenderableCollection------------------------------//

QueuedRenderableCollection::QueuedRenderableCollection() {
    
}

QueuedRenderableCollection::~QueuedRenderableCollection() {
    PassGroupRenderableMap::iterator i;
    for (i=mPassGrouped.begin(); i!=mPassGrouped.end(); ++i) {
        i->second->clear();
        delete i->second;
    }
    mPassGrouped.clear();
}

void QueuedRenderableCollection::addRenderable(Pass* pass, Renderable* renderable) {
    RenderableList* list;
    auto iter = mPassGrouped.find(pass);
    if (iter == mPassGrouped.end()) {
        list = new RenderableList();
        mPassGrouped.insert(PassGroupRenderableMap::value_type(pass, list));
    } else {
        list = iter->second;
    }
    list->push_back(renderable);
}

QueuedRenderableCollection::PassGroupRenderableMap& QueuedRenderableCollection::getPassGroupsRenderable() {
    return mPassGrouped;
}

void QueuedRenderableCollection::clear() {
    PassGroupRenderableMap::iterator i;
    for (i=mPassGrouped.begin(); i!=mPassGrouped.end(); ++i) {
        i->second->clear();
    }
    mPassGrouped.clear();
}




