//
//  SimMeshManager.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimMeshManager.hpp"
#include "SimMesh.hpp"
#include "SimVertexArrayManager.hpp"

template<> MeshManager *Singleton<MeshManager>::mSingleton = nullptr;


MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{
    _cache.clear();
}

MeshPtr MeshManager::getMesh(const String &name)
{
    auto it = _cache.find(name);
    if (it == _cache.end())
        return nullptr;
        
    return it->second;
}

MeshPtr MeshManager::loadMesh(const String& name)
{
    // Create mesh and load it.
    Mesh* mesh = createMesh(name);
    if (!mesh)
    {
        LogManager::getSingleton().debug("MeshManager::loadMesh", "load the " + name + "of mesh failed...");
        return nullptr;
    }
    
    
    MeshPtr ptr(mesh);
    
    // Cache it.
    _cache.insert(MeshCache::value_type(name, ptr));
    
    return ptr;
}

bool MeshManager::removeMesh(String& name)
{
    return _cache.erase(name);
}


Mesh* MeshManager::createMesh(const String& name)
{
    // For assimp lib to read file.
    int assimpVertComps = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs;
    
    Assimp::Importer import;
    const aiScene *ai_scene = import.ReadFile(MODEL_RESOURCE_DIR + name, assimpVertComps);
    
    if (!ai_scene || ai_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
    {
        LogManager::getSingleton().debug("MeshManager::load", import.GetErrorString());
        return nullptr;
    }
    
    Mesh* mesh = new Mesh(name);
    processAiNode(ai_scene, ai_scene->mRootNode, mesh);
    
    return mesh;
}

void MeshManager::processAiNode(const aiScene* ai_scene, aiNode* ai_node, Mesh* mesh)
{
    // Process meshes of current node.
    for (unsigned int i = 0; i < ai_node->mNumMeshes; i++)
    {
        aiMesh *ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
        processAiMesh(ai_scene, ai_mesh, mesh);
    }
    // Process node recursively.
    for (unsigned int i = 0; i < ai_node->mNumChildren; i++)
    {
        processAiNode(ai_scene, ai_node->mChildren[i], mesh);
    }
}

void MeshManager::processAiMesh(const aiScene* ai_scene, aiMesh* ai_mesh, Mesh* mesh)
{
    if (ai_mesh->mNumVertices <= 0)
        return;
    
    // Create vertex array object.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    
    glBindVertexArray(vao);
    
    // Deal the components of vertex.
    VertexDataDeclare* vdd = new VertexDataDeclare();
    vdd->addElement(VES_POSITION, VET_FLOAT3);
    vdd->addElement(VES_NORMAL, VET_FLOAT3);
    vdd->addElement(VES_TANGENT, VET_FLOAT3);
    vdd->addElement(VES_TEXCOORD, VET_FLOAT3);
    
    // Vertex data.
    VertexData* vertexData = new VertexData(vdd);
    vertexData->setNumberVertices(ai_mesh->mNumVertices);
    
    // Initializing vertex gpu buffer.
    HardwareVertexBuffer* hvb = vertexData->createBuffer(vdd->getStride()*ai_mesh->mNumVertices, HardwareBuffer::Usage::HBU_STATIC);
    
    const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
    Vector<float> vertices;
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        const aiVector3D v3dPos = ai_mesh->mVertices[i];
        const aiVector3D v3dNormal = ai_mesh->mNormals[i];
        const aiVector3D v3dTangent = ai_mesh->mTangents[i];
        const aiVector3D v3dTexCoord = ai_mesh->HasTextureCoords(0)? ai_mesh->mTextureCoords[0][i] : aiZeroVector;
        
        vertices.push_back(v3dPos.x);vertices.push_back(v3dPos.y);vertices.push_back(v3dPos.z);
        vertices.push_back(v3dNormal.x);vertices.push_back(v3dNormal.y);vertices.push_back(v3dNormal.z);
        vertices.push_back(v3dTangent.x);vertices.push_back(v3dTangent.y);vertices.push_back(v3dTangent.z);
        vertices.push_back(v3dTexCoord.x);vertices.push_back(v3dTexCoord.y);vertices.push_back(v3dTexCoord.z);
    }
    
    hvb->writeData(0, hvb->getSize(), &vertices[0]);
    // Release.
    vertices.clear();
    vertices.shrink_to_fit();
    
    // Index data.
    IndexData* indexData = new IndexData();
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        aiFace ai_face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < ai_face.mNumIndices; ++j)
        {
            indices.push_back(ai_face.mIndices[j]);
        }
    }
    
    // Initializing vertex index gpu buffer.
    HardwareIndexBuffer* hib = indexData->createBuffer(HardwareIndexBuffer::IndexType::IT_UInt, indices.size(), HardwareBuffer::Usage::HBU_STATIC);

    hib->writeData(0, hib->getSize(), &indices[0]);
    
    // Release.
    indices.clear();
    indices.shrink_to_fit();
    
    glBindVertexArray(0);
    
    // Record the vao object.
    SimUInt64 key = hvb->getBufferId();
    key += static_cast<SimUInt64>(hib->getBufferId()) << 32;
    VertexArrayManager::getSingleton().cacheVao(key, vao);

    // Create submesh.
    SubMesh* subMesh = new SubMesh(vertexData, indexData);
    // Add it to mesh.
    mesh->addSubMesh(subMesh);
    
    // Texture data.
    if (ai_mesh->mMaterialIndex > 0)
    {
        aiMaterial *mtl = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        processAiMaterial(mtl, aiTextureType_AMBIENT, "texture_ambient", subMesh);
        processAiMaterial(mtl, aiTextureType_DIFFUSE, "texture_diffuse", subMesh);
        processAiMaterial(mtl, aiTextureType_SPECULAR, "texture_specular", subMesh);
        processAiMaterial(mtl, aiTextureType_EMISSIVE, "texture_emissive", subMesh);
    }
}

void MeshManager::processAiMaterial(aiMaterial *mtl, const aiTextureType type, const String &typeName, SubMesh* subMesh)
{
    int count = mtl->GetTextureCount(type);
    aiString filename;
    for (unsigned int i = 0; i < count; i++)
    {
        if (mtl->GetTexture(type, i, &filename) != AI_SUCCESS)
        {
            continue;
        }
        const String name     = filename.data;
        const String realName = name.substr(2, name.length());
        // Get texture.
//        Texture *tex = load2DTexture(realName);
    }
}
