//
//  SimMovableObject.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/9.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimMovableObject_hpp
#define SimMovableObject_hpp

#include "SimCommon.hpp"

class SceneManager;
class RenderQueue;
class Node;
class SceneNode;

class MovableObject {
public:
    MovableObject(const String& name);
    virtual ~MovableObject();
    
protected:
    SceneManager* mManager;
    String mName;
    Node* mParent;
    bool mVisible;
    
    int mQueueID;
    
public:
    virtual const String& getName();
    
    virtual SceneManager* getSceneManager();
    
    virtual Node* getParentNode();
    virtual SceneNode* getParentSceneNode();
    
    virtual void setRenderQueueGroup(const int queueID);
    virtual const int getRenderQueueGroup();
    
    virtual void setVisible(bool visible);
    virtual const bool getVisible();
    
    virtual void updateRenderQueue(RenderQueue* queue) = 0;
};

#endif /* SimMovableObject_hpp */
