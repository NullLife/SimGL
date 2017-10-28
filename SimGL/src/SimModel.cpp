//
// Created by yangbin on 2017/4/14.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimModel.hpp"

#include "SimSceneNode.hpp"
#include "SimMesh.hpp"
#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"

#include "SimGLRenderSystem.hpp"
#include "SimGLProgram.hpp"
#include "SimParameterDataSource.hpp"
#include "SimTransform.hpp"

#include "SimMaterialManager.hpp"
#include "SimGLProgramManager.hpp"

Model::Model(const String &name) :
    MovableObject(name),
    _polyMode(PM_FILL),
    _drawType(DT_TRIANGLES),
    _bbVertexData(nullptr),
    _bbIndexData(nullptr),
    _bbMaterial(nullptr),
    _bbVao(0),
    _needUpdateBBBufer(true)
{
    _meshPtr = _loadMesh(name);
}

Model::Model(Mesh* mesh) :
    MovableObject(mesh->getName()),
    _meshPtr(MeshPtr(mesh)),
    _polyMode(PM_FILL),
    _drawType(DT_TRIANGLES),
    _bbVertexData(nullptr),
    _bbIndexData(nullptr),
    _bbMaterial(nullptr),
    _bbVao(0),
    _needUpdateBBBufer(true)
{
    _buildSubModelList(_meshPtr);
}


Model::~Model()
{
    LogManager::getSingleton().debug("Delete Model", "name: " + mName);
    SubModelList::iterator i = _smList.begin();
    for (; i!=_smList.end(); ++i)
    {
        delete (*i);
    }
    _smList.clear();
    _smList.shrink_to_fit();
    
    if (_bbVertexData)
        delete _bbVertexData;
    
    if (_bbIndexData)
        delete _bbIndexData;
    
    if (_bbVao > 0)
        glDeleteVertexArrays(1, &_bbVao);
    
    if (_bbMaterial)
        MaterialManager::getSingleton().remove(_bbMaterial->getName());
}

const MeshPtr& Model::getMesh()
{
    return _meshPtr;
}

void Model::calcBoundingBox()
{
    std::vector<Vec3> allPosData;
    const Mesh::SubMeshList& list = _meshPtr->getSubMeshList();
    VertexData* vd;
    for (unsigned int i=0; i<list.size(); ++i)
    {
        vd = list[i]->getVertexData();
        HardwareVertexBuffer* buffer = vd->getBuffer();
        std::vector<Vec3> posData(vd->getNumVertices());
        
        buffer->readData(0, buffer->getSize(),
                         vd->getVertexDataDeclare()->getStride(), 0, VertexElement::getVertexElementOffset(VET_FLOAT3),
                         (void *)&posData[0]);
        
//        for (int j=0; j<posData.size(); ++j)
//        {
//            _bb.expandBy(posData[j]);
//        }
        
        allPosData.insert(allPosData.end(), posData.begin(), posData.end());
        
        posData.clear();
    }
    
    // Conbine all children box.
    _obb = OrientedBoundingBox::create(&allPosData[0], allPosData.size());
    
    float tNear = 0, tFar = FLT_MAX;
    OrientedBoundingBox::Ray ray;
    ray._pos = Vec3(2, 3, 4);
    ray._dir = Vec3(1, 3, 0);
    ray._dir = glm::normalize(ray._dir);
    bool ret = _obb.intersectLine(ray, tNear, tFar);
    if (ret)
    {
        Vec3 intersectPoint = ray._pos + tNear * ray._dir;
        printf("obb intersectPoint: %s \n", StringUtils::toString(intersectPoint).c_str());
    }
    
    printf("obb center: %s \n", StringUtils::toString(_obb._position).c_str());
    printf("obb axis1: %f * %s \n", _obb._halfExtents.x, StringUtils::toString(_obb._orientation[0]).c_str());
    printf("obb axis2: %f * %s \n", _obb._halfExtents.y, StringUtils::toString(_obb._orientation[1]).c_str());
    printf("obb axis3: %f * %s \n", _obb._halfExtents.z, StringUtils::toString(_obb._orientation[2]).c_str());
}

void Model::setMaterial(const MaterialPtr& mtlPtr)
{
    for (SubModel* subModel : _smList)
    {
        subModel->setMaterial(mtlPtr);
    }
}

void Model::setMaterial(const String& material)
{
    setMaterial(MaterialManager::getSingleton().get(material));
}

const Model::SubModelList &Model::getSubModels()
{
    return _smList;
}

void Model::setRenderQueueGroup(const int queueID)
{
    MovableObject::setRenderQueueGroup(queueID);
    for (SubModel* sub : _smList)
    {
        sub->setRenderQueueGroup(queueID);
    }
}

void Model::updateRenderQueue(RenderQueue* queue)
{
    for (SubModel* sub : _smList)
    {
        if (sub->isVisible())
        {
            queue->addRenderable(sub);
        }
    }
}

MeshPtr Model::_loadMesh(const String& name)
{
    MeshPtr mesh = MeshManager::getSingleton().getMesh(name);

    _buildSubModelList(mesh);
    
    return mesh;
}

void Model::_buildSubModelList(const MeshPtr& mesh)
{
    if (!mesh)
        LogManager::getSingleton().debug("Model::buildSubModelList","mesh is null");
    
    const Mesh::SubMeshList& subMeshList = mesh->getSubMeshList();
    _smList.resize(subMeshList.size());
    
    int index = 0;
    for (SubMesh* subMesh : subMeshList)
    {
        SubModel* subModel = new SubModel(this);
        subModel->setSubMesh(subMesh);
        
        _smList[index] = subModel;
        ++index;
    }
}

void Model::createBoundingBoxBuffer()
{
    if (!_obb.valid() && _bbVertexData && _bbIndexData && _needUpdateBBBufer)
        return;
    
    VertexDataDeclare* vdd = new VertexDataDeclare();
    vdd->addElement(VES_POSITION, VET_FLOAT3);
    
    Vector<Vec3> vertices;
    for (int i=0; i<8; ++i)
        vertices.push_back(_obb.cornerPoint(i));
    
    Vector<unsigned int> indices;
    indices.push_back(0);indices.push_back(4);indices.push_back(5);indices.push_back(1);
    indices.push_back(0);indices.push_back(4);indices.push_back(6);indices.push_back(2);
    indices.push_back(0);indices.push_back(2);indices.push_back(3);indices.push_back(1);
    
    indices.push_back(7);indices.push_back(3);indices.push_back(2);indices.push_back(6);
    indices.push_back(7);indices.push_back(6);indices.push_back(4);indices.push_back(5);
    indices.push_back(7);indices.push_back(5);indices.push_back(1);indices.push_back(3);
    
    glGenVertexArrays(1, &_bbVao);
    glBindVertexArray(_bbVao);
    
    _bbVertexData = new VertexData(vdd);
    HardwareVertexBuffer* vb =  _bbVertexData->createBuffer(vdd->getStride(), vertices.size(), HardwareBuffer::Usage::HBU_STATIC);
    vb->writeData(&vertices[0]);
    
    _bbIndexData = new IndexData();
    HardwareIndexBuffer* ib = _bbIndexData->createBuffer(HardwareIndexBuffer::IndexType::IT_UInt, indices.size(), HardwareBuffer::Usage::HBU_STATIC);
    ib->writeData(&indices[0]);
    
    glBindVertexArray(0);
    
    _bbMaterial = MaterialManager::getSingleton().get("box.material");
    if (_bbMaterial == nullptr)
        LogManager::getSingleton().error("Loading box's material error...");
    
    _needUpdateBBBufer = true;
}

void Model::renderBoundingBox(GLRenderSystem* rs)
{
    Pass* pass = _bbMaterial->getBestTechnique()->getPass(0);
    rs->bindVertexShader(pass->getVertexShader());
    rs->bindFragmentShader(pass->getFragmentShader());
    
    GLProgram* activeProgram = GLProgramManager::getSingleton().getActiveProgram();
    
    glBindVertexArray(_bbVao);
    
    _bbVertexData->bind();
    
    Mat4 mm = mParent->getTransform()->getModelMatrix();
    rs->getSceneManager()->getParameterDataSource()->setModelMatrix(mm);
    rs->getSceneManager()->getParameterDataSource()->setCurrentPass(pass);
    
    rs->updateProgramParameters(pass);
    
    activeProgram->updateUniforms();
    
    glPolygonMode(GL_FRONT_AND_BACK, PolygonMode::PM_LINE);
    
    glDrawElements(GL_TRIANGLE_FAN, (GLsizei) _bbIndexData->getNumIndices(), _bbIndexData->getBuffer()->getIndexType(), 0);
    
    glPolygonMode(GL_FRONT_AND_BACK, PolygonMode::PM_FILL);
}
