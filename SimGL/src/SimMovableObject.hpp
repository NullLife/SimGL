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
#include "SimBoundingBox.hpp"
#include "SimOrientedBoundingBox.hpp"

class SceneManager;
class RenderQueue;
class Node;
class SceneNode;

class GLRenderSystem;

class MovableObject
{
public:
    MovableObject(const String& name);
    virtual ~MovableObject();
    
    virtual const String& getName();
    
    virtual SceneManager* getSceneManager();
    
    virtual void setParent(Node* node);
    virtual Node* getParentNode();
    virtual SceneNode* getParentSceneNode();
    
    virtual const BoundingBox& getBoundingBox();
    virtual void calcBoundingBox() {}
    virtual void createBoundingBoxBuffer() {}
    virtual void renderBoundingBox(GLRenderSystem* rs) {}
    
    virtual void setRenderQueueGroup(const int queueID);
    virtual const int getRenderQueueGroup();
    
    virtual void setVisible(bool visible);
    virtual const bool getVisible();
    
    virtual void updateRenderQueue(RenderQueue* queue) = 0;
    
protected:
    SceneManager* _sceneManager;
    String _name;
    Node* _parent;
    bool _visible;

    int _queueID;
    
    BoundingBox _bb;
    OrientedBoundingBox _obb;
};

#endif /* SimMovableObject_hpp */
