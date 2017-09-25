//
// Created by yangbin on 2017/2/22.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef CLEBGL_RENDERQUEUE_HPP
#define CLEBGL_RENDERQUEUE_HPP

#include "SimCommon.hpp"

class RenderQueueGroup;
class Renderable;

enum RenderQueueGroupID {
    RQT_BACKGROUND = 0,
    RQT_SKIES = 10,
    RQT_NORMAL = 20,
};

class RenderQueue {
public:
    struct LessGroup {
        bool operator()(const int& k1, const int& k2) const {
            return k1 < k2;
        }
    };

    typedef OrderHashMap<unsigned int, RenderQueueGroup*, LessGroup> RenderQueueGroupMap;
public:
    RenderQueue();
    ~RenderQueue();

private:
    // Group
    RenderQueueGroupMap mQueueGroupMap;

public:
    void addRenderable(Renderable* renderable, RenderQueueGroupID groupID = RQT_NORMAL);

    /**
     * Get or create RenderQueueGroup.
     * @param groupID The ID for render queue
     * @return render queue
     */
    RenderQueueGroup* getRenderQueueGroup(RenderQueueGroupID groupID);

    RenderQueueGroup* getDefaultRenderQueueGroup();

    RenderQueueGroupMap& getRenderQueueGroups();

    /**
     * Clear render queue.
     */
    void clear();
};


#endif //CLEBGL_RENDERQUEUE_HPP
