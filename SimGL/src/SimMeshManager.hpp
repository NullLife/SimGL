//
//  SimMeshManager.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimMeshManager_hpp
#define SimMeshManager_hpp

#include "SimSingleton.hpp"
#include "SimCommon.hpp"
#include "SimVertexIndexData.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class SubMesh;
class Material;
class Texture;

class MeshManager : public Singleton<MeshManager>
{
public:
    typedef HashMap<String, MeshPtr> MeshCache;
    
    MeshManager();
    ~MeshManager();

private:
    MeshCache _cache;
    
public:
    // Maybe return null if the cache don't eixt it.
    MeshPtr getMesh(const String& name);
    
    // Load new mesh.
    MeshPtr loadMesh(const String& name);
    
    // Remove it from cache.
    bool removeMesh(String& name);
    
private:
    Mesh* createMesh(const String& name);
    void processAiNode(const aiScene* ai_scene, aiNode* ai_node, Mesh* mesh);
    void processAiMesh(const aiScene* ai_scene, aiMesh* ai_mesh, Mesh* mesh);
    void processAiMaterial(aiMaterial *mtl, const aiTextureType type, const String &typeName, SubMesh* subMesh);
};

#endif /* SimMeshManager_hpp */
