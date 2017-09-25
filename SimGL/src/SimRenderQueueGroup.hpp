//
// Created by yangbin on 2017/2/22.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef CLEBGL_RENDERQUEUEGROUP_HPP
#define CLEBGL_RENDERQUEUEGROUP_HPP


#include "SimCommon.hpp"
#include "SimRenderQueue.hpp"

class Renderable;
class Technique;
class Pass;

//-------------QueuedRenderableCollection--------------//
struct RenderablePass {
    /// Pointer to the Renderable details
    Renderable* mRenderable;
    /// Pointer to the Pass
    Pass* mPass;
    
    RenderablePass(Renderable* renderable, Pass* pass) : mRenderable(renderable), mPass(pass) {}
};


class QueuedRenderableCollection {
public:
//    typedef Vector<RenderablePass> RenderablePassList;
    typedef Vector<Renderable *> RenderableList;
    typedef OrderHashMap<Pass*, RenderableList*> PassGroupRenderableMap;
public:
    QueuedRenderableCollection();
    ~QueuedRenderableCollection();
    
private:
    PassGroupRenderableMap mPassGrouped;
    
public:
    void addRenderable(Pass* pass, Renderable* renderable);
    PassGroupRenderableMap& getPassGroupsRenderable();
    void clear();
};


//-------------RenderPriorityGroup--------------//

class RenderPriorityGroup {
public:
    RenderPriorityGroup(RenderQueueGroup* parent);
    ~RenderPriorityGroup();
    
private:
    RenderQueueGroup* mParent;
    QueuedRenderableCollection mSoildCollection;
    
public:
    void addSolidRenderable(Renderable *renderable, Technique* tech);
    
    QueuedRenderableCollection& getSolidCollection();
    
    void clear();
};

//-------------RenderQueueGroup--------------//

class RenderQueueGroup {
public:
    struct LessProrioty {
        bool operator()(const short& k1, const short& k2) const {
            return k1 < k2;
        }  
    };
    typedef OrderHashMap<short, RenderPriorityGroup*, LessProrioty> PriorityMap;
public:
    RenderQueueGroup(RenderQueueGroupID groupID);

    ~RenderQueueGroup();

private:
    // ID
    RenderQueueGroupID mGroupID;

    PriorityMap mPriorityGroups;
public:
    const RenderQueueGroupID& getGroupID() const;
    
    void addRenderable(Renderable *renderable, Technique* tech, short priority);
    
    PriorityMap& getRenderPriorityGroups();
    
    void clear();
};


#endif //CLEBGL_RENDERQUEUEGROUP_HPP
