//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_MODEL_HPP
#define SIMGL_MODEL_HPP

#include "SimMeshManager.hpp"
#include "SimMovableObject.hpp"
#include "SimRenderOperation.hpp"
#include "SimSubModel.hpp"

class Mesh;
class SceneNode;

class Model : public MovableObject
{
public:
    typedef Vector<SubModel*> SubModelList;
    
public:
    Model(const String &name);
    Model(Mesh* mesh);
    ~Model();

    const String& getName() { return mName; }
    
    const MeshPtr& getMesh();
    
    void calcBoundingBox();
    void createBoundingBoxBuffer();
    void renderBoundingBox(GLRenderSystem* rs);
    
    void setMaterial(const MaterialPtr& mtlPtr);
    void setMaterial(const String& material);
    
    void setPolygonMode(const PolygonMode mode) { _polyMode = mode; }
    const PolygonMode getPolygonMode() const { return _polyMode; }
    
    void setDrawType(const DrawType type) { _drawType = type; }
    const DrawType getDrawType() const { return _drawType; }

    const SubModelList& getSubModels();
    
    void setRenderQueueGroup(const int queueID);
    
    void updateRenderQueue(RenderQueue* queue);

private:
    MeshPtr _loadMesh(const String& name);
    
    void _buildSubModelList(const MeshPtr& meshPtr);
    
    MeshPtr _meshPtr;
    SubModelList _smList;
    PolygonMode _polyMode;
    DrawType _drawType;
    
    VertexData* _bbVertexData;
    IndexData* _bbIndexData;
    MaterialPtr _bbMaterial;
    GLuint _bbVao;
    bool _needUpdateBBBufer;
};


#endif //SIMGL_MODEL_HPP
