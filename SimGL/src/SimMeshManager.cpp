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


MeshManager::MeshManager() {}

MeshManager::~MeshManager() {
    mCache.clear();
}

MeshManager::MeshPtr MeshManager::getMesh(const String &name) {
    auto it = mCache.find(name);
    if (it != mCache.end()) {
        return it->second;
    }
    
    // Create mesh and load it.
    Mesh* mesh = createMesh(name);
    MeshPtr ptr(mesh);
    if (!ptr) {
        return ptr;
    }
    // Cache it.
    mCache.insert(MeshCache::value_type(name, ptr));
    
    return ptr;
}


Mesh* MeshManager::createMesh(const String& name) {
    Assimp::Importer import;
    const aiScene *ai_scene = import.ReadFile(MODEL_RESOURCE_DIR + name,
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs |
                                              aiProcess_CalcTangentSpace);
    
    if (!ai_scene || ai_scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode) {
        LogManager::getSingleton().debug("MeshManager::load", import.GetErrorString());
        return nullptr;
    }
    
    Mesh* mesh = new Mesh(name);
    processAiNode(ai_scene, ai_scene->mRootNode, mesh);
    
    return mesh;
}

void MeshManager::processAiNode(const aiScene* ai_scene, aiNode* ai_node, Mesh* mesh) {
    // Process meshes of current node.
    for (unsigned int i = 0; i < ai_node->mNumMeshes; i++) {
        aiMesh *ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
        processAiMesh(ai_scene, ai_mesh, mesh);
    }
    // Process node recursively.
    for (unsigned int i = 0; i < ai_node->mNumChildren; i++) {
        processAiNode(ai_scene, ai_node->mChildren[i], mesh);
    }
}

void MeshManager::processAiMesh(const aiScene* ai_scene, aiMesh* ai_mesh, Mesh* mesh) {
    if (ai_mesh->mNumVertices <= 0) {
        return;
    }
    // Vertex data declare. [position, normal, tangent, texcoord]
    VertexDataDeclare* vertexDD = new VertexDataDeclare();
    vertexDD->addElement(new VertexElement(VES_POSITION, VET_FLOAT3));
    vertexDD->addElement(new VertexElement(VES_NORMAL, VET_FLOAT3));
    vertexDD->addElement(new VertexElement(VES_TANGENT, VET_FLOAT3));
    vertexDD->addElement(new VertexElement(VES_TEXCOORD, VET_FLOAT2));
    
    // Create vertex array object.
    GLuint vao;
    glGenVertexArrays(1, &vao);
    
    glBindVertexArray(vao);
    
    // Vertex data.
    VertexData* vertexData = new VertexData(vertexDD);
    // Initializing vertex gpu buffer.
    vertexData->initBuffer(vertexDD->getStride(), ai_mesh->mNumVertices, HardwareBuffer::Usage::HBU_STATIC);
    HardwareVertexBuffer* hvb = vertexData->getBuffer();
    
    const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
    Vector<float> vertices;
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        // Pos
        const aiVector3D v3dPos = ai_mesh->mVertices[i];
        // Normal
        const aiVector3D v3dNormal = ai_mesh->mNormals[i];
        // Tangent
        const aiVector3D v3dTangent = ai_mesh->mTangents[i];
        // Tex
        const aiVector3D v3dTexCoord = ai_mesh->HasTextureCoords(0)? ai_mesh->mTextureCoords[0][i] : aiZeroVector;
        
        vertices.push_back(v3dPos.x);vertices.push_back(v3dPos.y);vertices.push_back(v3dPos.z);
        vertices.push_back(v3dNormal.x);vertices.push_back(v3dNormal.y);vertices.push_back(v3dNormal.z);
        vertices.push_back(v3dTangent.x);vertices.push_back(v3dTangent.y);vertices.push_back(v3dTangent.z);
        vertices.push_back(v3dTexCoord.x);vertices.push_back(v3dTexCoord.y);

//        size_t offset = 0;
//        hvb->writeData(i*vertexDD->getStride(), sizeof(v3dPos), &v3dPos);
//        offset = sizeof(Vec3);
//        
//        hvb->writeData(i*vertexDD->getStride() + offset, sizeof(v3dNormal), &v3dNormal);
//        offset += sizeof(Vec3);
//        
//        hvb->writeData(i*vertexDD->getStride() + offset, sizeof(v3dTangent), &v3dTangent);
//        offset += sizeof(Vec3);
//        
//        Vec2 texCoord = Vec2(v3dTexCoord.x, v3dTexCoord.y);
//        hvb->writeData(i*vertexDD->getStride() + offset, sizeof(texCoord), &texCoord[0]);
//        offset += sizeof(Vec2);
    }
    hvb->writeData(0, hvb->getSize(), &vertices[0]);
    // Release.
    vertices.clear();
    vertices.shrink_to_fit();
    
    // Index data.
    IndexData* indexData = new IndexData();
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
        aiFace ai_face = ai_mesh->mFaces[i];
        for (unsigned int j = 0; j < ai_face.mNumIndices; ++j) {
            indices.push_back(ai_face.mIndices[j]);
        }
    }
    
    // Initializing vertex index gpu buffer.
    indexData->initBuffer(HardwareIndexBuffer::IndexType::IT_UInt, indices.size(), HardwareBuffer::Usage::HBU_STATIC);
    HardwareIndexBuffer* hib = indexData->getBuffer();
    hib->writeData(0, hib->getSize(), &indices[0]);
    
    // Release.
    indices.clear();
    indices.shrink_to_fit();
    
    // Record the vao object.
    SimUInt64 key = hvb->getBufferId();
    key += static_cast<SimUInt64>(hib->getBufferId()) << 32;
    VertexArrayManager::getSingleton().cacheVao(key, vao);

    // Create submesh.
    SubMesh* subMesh = new SubMesh(vertexData, indexData);
    // Add it to mesh.
    mesh->addSubMesh(subMesh);
    
    // Texture data.
    if (ai_mesh->mMaterialIndex > 0) {
        aiMaterial *mtl = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        processAiMaterial(mtl, aiTextureType_AMBIENT, "texture_ambient", subMesh);
        processAiMaterial(mtl, aiTextureType_DIFFUSE, "texture_diffuse", subMesh);
        processAiMaterial(mtl, aiTextureType_SPECULAR, "texture_specular", subMesh);
        processAiMaterial(mtl, aiTextureType_EMISSIVE, "texture_emissive", subMesh);
    }
}

void MeshManager::processAiMaterial(aiMaterial *mtl, const aiTextureType type, const String &typeName, SubMesh* subMesh) {
    int count = mtl->GetTextureCount(type);
    aiString filename;
    for (unsigned int i = 0; i < count; i++) {
        if (mtl->GetTexture(type, i, &filename) != AI_SUCCESS) {
            continue;
        }
        const String name     = filename.data;
        const String realName = name.substr(2, name.length());
        // Get texture.
//        Texture *tex = load2DTexture(realName);
    }
}













