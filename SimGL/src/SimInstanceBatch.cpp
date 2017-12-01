//
//  SimInstanceBatch.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimInstanceBatch.hpp"

#include "SimMeshManager.hpp"
#include "SimMaterialManager.hpp"
#include "SimVertexArrayManager.hpp"

#include "SimMesh.hpp"
#include "SimSubMesh.hpp"

#include "SimInstancedModel.hpp"

#include "SimRenderQueue.hpp"

InstanceBatch::InstanceBatch(const String &name, const String& meshName, const String& mtlName) :
    Renderable(),
    MovableObject(name),
    _isBuilded(false),
    _meshName(meshName),
    _mtlName(mtlName),
    _numInstancesPerBatch(1)
{
}

InstanceBatch::~InstanceBatch()
{
    InstanceModelList::iterator modelIter = _instances.begin();
    for (; modelIter != _instances.end(); ++modelIter)
        delete (*modelIter);
    _instances.clear();
    _instances.shrink_to_fit();
    
    BatchInstanceList::iterator batchIter = _batchInstances.begin();
    for (; batchIter != _batchInstances.end(); ++batchIter)
        delete (*batchIter);
    _batchInstances.clear();
    _batchInstances.shrink_to_fit();
}

void InstanceBatch::createAllInstances()
{
    _instances.resize(_numInstancesPerBatch);
    _instances.reserve(_numInstancesPerBatch);
    for (unsigned int i=0; i<_numInstancesPerBatch; ++i)
    {
        InstancedModel* ins = new InstancedModel(this, i);
        _instances[i] = ins;
    }
}

void InstanceBatch::build()
{
    if (_isBuilded)
        return;
    
    // Load material.
    _sharedMtl = MaterialManager::getSingleton().get(_mtlName);
    
    // Load mesh.
    _sharedMesh = MeshManager::getSingleton().getMesh(_meshName);
    if (_sharedMesh == nullptr)
        _sharedMesh = MeshManager::getSingleton().loadMesh(_meshName);
    
    // Construct batch.
    const Mesh::SubMeshList& subMeshList = _sharedMesh->getSubMeshList();
    _batchInstances.resize(subMeshList.size());
    _batchInstances.reserve(subMeshList.size());
    
    for (unsigned int i=0; i<subMeshList.size(); ++i)
    {
        BatchInstance* batch = new BatchInstance(this, i);
        _batchInstances[i] = batch;
        _buildFrom(subMeshList[i], batch);
    }
    
    _isBuilded = true;
}

void InstanceBatch::_buildFrom(SubMesh* sub, BatchInstance* batch)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    
    // Get vertices data from GPU.
    VertexData* vd = sub->getVertexData();
    // Get indices data from GPU.
    IndexData* idx = sub->getIndexData();
    
    const VertexDataDeclare* vdd = vd->getVertexDataDeclare();
    HardwareVertexBuffer* verBuf = vd->getBuffer();
    
    Vector<float> positions;
    Vector<float> texCoords;
    Vector<unsigned int> indices;
    
    if (vdd->getVertexElementLayout() == VEL_CROSS)
    {
        positions.resize(vd->getNumberVertices() * 3);
        positions.reserve(vd->getNumberVertices() * 3);
        verBuf->readData(0, verBuf->getSize(),
                         vdd->getStride(),
                         0, VertexElement::getVertexElementOffset(VET_FLOAT3),
                         (void *)&positions[0]);
        
        texCoords.resize(vd->getNumberVertices() * 3);
        texCoords.reserve(vd->getNumberVertices() * 3);
        verBuf->readData(0, verBuf->getSize(),
                         vdd->getStride(),
                         vdd->getElementOffset(VES_TEXCOORD), VertexElement::getVertexElementOffset(VET_FLOAT3),
                         (void *)&texCoords[0]);
        if (idx)
        {
            indices.resize(idx->getNumberIndices());
            HardwareIndexBuffer* idxBuf = idx->getBuffer();
            idxBuf->readData(0, idxBuf->getSize(), idxBuf->getIndexSize(),
                             0, idxBuf->getIndexSize(),
                             &indices[0]);
        }
    }
    
    // Binding New vertex array object.
    glBindVertexArray(vao);
    
    VertexDataDeclare* instanceVDD = new VertexDataDeclare(VEL_ALIGN);                    // Using align layout.
    instanceVDD->addElement(VES_POSITION, VET_FLOAT3, vd->getNumberVertices());           // Position
    instanceVDD->addElement(VES_TEXCOORD, VET_FLOAT3, vd->getNumberVertices());           // TexCoord
    instanceVDD->addElement(VES_INSTANCE_POSITION, VET_FLOAT3, _numInstancesPerBatch);    // Instance position
    
    Vector<float> instanceData;
    instanceData.insert(instanceData.end(), positions.begin(), positions.end());
    instanceData.insert(instanceData.end(), texCoords.begin(), texCoords.end());
    
    for (int i=0; i<_numInstancesPerBatch; ++i)
    {
        instanceData.push_back(_instances[i]->getTransform()->getPosition().x);
        instanceData.push_back(_instances[i]->getTransform()->getPosition().y);
        instanceData.push_back(_instances[i]->getTransform()->getPosition().z);
    }
    size_t bufSize = sizeof(float) * instanceData.size();
    VertexData* instanceVerData = new VertexData(instanceVDD);
    HardwareVertexBuffer* insVerBuf = instanceVerData->createBuffer(bufSize, HardwareBuffer::Usage::HBU_STATIC);
    insVerBuf->writeData((void*) &instanceData[0]);
    
    instanceVerData->setNumberVertices(vd->getNumberVertices());
    batch->setVertexData(instanceVerData);
    
    SimUInt64 key = insVerBuf->getBufferId();
    
    // If there are indice data.
    if (idx)
    {
        IndexData* instanceIdxData = new IndexData();
        HardwareIndexBuffer* insIdxBuf = instanceIdxData->createBuffer(HardwareIndexBuffer::IndexType::IT_UInt, idx->getNumberIndices(), HardwareBuffer::Usage::HBU_STATIC);
        insIdxBuf->writeData((void *) &indices[0]);
        
        batch->setIndexData(instanceIdxData);
        
        key += static_cast<SimUInt64>(insIdxBuf->getBufferId()) << 32;
    }
    
    // Cache vao
    VertexArrayManager::getSingleton().cacheVao(key, vao);
    
    positions.clear();
    texCoords.clear();
    indices.clear();
    instanceData.clear();
}


void InstanceBatch::updateRenderQueue(RenderQueue* queue)
{
    BatchInstanceList::iterator it = _batchInstances.begin();
    for (; it != _batchInstances.end(); ++it)
        queue->addRenderable(*it);
}

BatchInstance::BatchInstance(InstanceBatch* parent, int index) :
    _parent(parent),
    _index(index),
    _vertexData(nullptr),
    _indexData(nullptr)
{
}

BatchInstance::~BatchInstance()
{
}

void BatchInstance::getRenderOperation(RenderOperation& op)
{
    op._obj = this;
    op._drawType = DrawType::DT_TRIANGLES;
    
    SimUInt64 vaoKey = 0;
    
    op._start = 0;
    op._vertexData = _vertexData;
    op._count = _vertexData->getNumberVertices();
    
    vaoKey = _vertexData->getBuffer()->getBufferId();
    
    op._useIndex = false;
    op._indexData = _indexData;
    if (_indexData)
    {
        op._useIndex = true;
        op._count = _indexData->getNumberIndices();
        vaoKey += static_cast<SimUInt64>(_indexData->getBuffer()->getBufferId()) << 32;
    }
    
    op._vao = VertexArrayManager::getSingleton().getVao(vaoKey);
    
    op._hasInstancesData = true;
    op._numOfInstances = _parent->getNumInstancePerBatch();
}


