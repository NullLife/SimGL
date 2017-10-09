//
// Created by yangbin on 2017/4/16.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_SUBMODEL_HPP
#define SIMGL_SUBMODEL_HPP

#include "SimCommon.hpp"
#include "SimRenderable.hpp"
#include "SimRenderQueue.hpp"

class Model;
class SubMesh;
class Material;

class SubModel : public Renderable
{
public:
    SubModel(Model* mParent);
    ~SubModel();

private:
    Model* mParent;
    SubMesh* mSubMesh;
    MaterialPtr mMtlPtr;
    bool mVisible;
    bool mNeedCommitBuffer;
    
    int mQueueID;

public:
    Model* getParent();

    void setSubMesh(SubMesh* subMesh);

    SubMesh* getSubMesh();

    void setMaterial(const MaterialPtr& mtl);

    const MaterialPtr& getMaterial();
    
    const Mat4& getWorldTransforms();
    
    void setVisible(bool visible);
    
    bool isVisible();

    void getRenderOperation(RenderOperation& op);
    
    void setRenderQueueGroup(int queueID);
    
    const int getRenderQueueGroup();
};


#endif //SIMGL_SUBMODEL_HPP
