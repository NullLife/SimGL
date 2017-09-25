//
//  SimCommon.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimCommon_hpp
#define SimCommon_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include "SimLogManager.hpp"
#include "SimStringUtils.hpp"

#ifndef String
#define String std::string
#endif

#ifndef Vector
#define Vector std::vector
#endif

#ifndef SharedPtr
#define SharedPtr std::shared_ptr
#endif

/////////////////////////

#ifndef HashMap
#define HashMap std::unordered_map
#endif

#ifndef OrderHashMap
#define OrderHashMap std::map
#endif

#ifndef Vec2
#define Vec2 glm::vec2
#endif

#ifndef Vec3
#define Vec3 glm::vec3
#endif

#ifndef IVec3
#define IVec3 glm::ivec3
#endif

#ifndef Vec4
#define Vec4 glm::vec4
#endif

#ifndef Mat4
#define Mat4 glm::mat4
#endif

#ifndef Mat3
#define Mat3 glm::mat3
#endif


#ifndef Quat
#define Quat glm::quat
#endif

///////////////////////

#ifndef RESOURCE_DIR
#define RESOURCE_DIR "/poject/gitpj/SimGL/SimGL/assets/"
#endif

#ifndef SCRIPT_RESOURCE_DIR
#define SCRIPT_RESOURCE_DIR "/poject/gitpj/SimGL/SimGL/assets/script/"
#endif


#ifndef MODEL_RESOURCE_DIR
#define MODEL_RESOURCE_DIR "/poject/gitpj/SimGL/SimGL/assets/model/"
#endif

#ifndef SHADER_RESOURCE_DIR
#define SHADER_RESOURCE_DIR "/poject/gitpj/SimGL/SimGL/assets/shader/"
#endif

#ifndef TEXTURE_RESOURCE_DIR
#define TEXTURE_RESOURCE_DIR "/poject/gitpj/SimGL/SimGL/assets/texture/"
#endif

///////////////////////

static const Vec3 X_AXIS = Vec3(1.0f, 0.0f, 0.0f);
static const Vec3 Y_AXIS = Vec3(0.0f, 1.0f, 0.0f);
static const Vec3 Z_AXIS = Vec3(0.0f, 0.0f, 1.0f);


class Texture;
class Material;
class Mesh;
class GLShader;

typedef SharedPtr<Material> MaterialPtr;
typedef SharedPtr<Mesh> MeshPtr;
typedef SharedPtr<Texture> TexturePtr;
typedef SharedPtr<GLShader> GLShaderPtr;
typedef Vector<String> StringVector;

typedef unsigned long long SimUInt64;

/////////////////////////

#endif /* SimCommon_hpp */
