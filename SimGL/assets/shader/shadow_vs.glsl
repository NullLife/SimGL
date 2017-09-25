#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

struct Transform {
    mat4 M;
    mat4 V;
    mat4 P;
    
    mat4 S;  // matrix tranform something to light space
};

struct Vertex {
    vec4 pos;
    vec3 normal;
    vec2 texCoord;
    
    vec4 shadowCoord; // in light
};

uniform Transform transform;

out Vertex vVertex;
out Transform vTrans;
out mat3 vTBN;

void main() {
    // Calculate view-space coordinate
    vVertex.pos = transform.V * transform.M * vec4(pos, 1.0);
    
    // Calculate light-space coordinate
    vVertex.shadowCoord = transform.S * transform.M* vec4(pos, 1.0);
    
    // Calculate view space normal
    mat3 normalMatrix = mat3(transpose(inverse(transform.V * transform.M)));
    vVertex.normal = normalize(normalMatrix * normal);
    
    vec3 t = normalize(normalMatrix * tangent);
    // Orthogonalization
    t = normalize(t - dot(vVertex.normal,t)*vVertex.normal);
    
    vec3 biTangent = cross(t, vVertex.normal);
    // 这个矩阵表示的是，切空间（tangent space）中的三个基向量被转换到当前坐标系下所对应的三个基向量构成的空间
    // 顶点多，代表NormalMap里面normal多，一般把光线转化到切空间里面去
    // 列
    vTBN = mat3(t, biTangent, vVertex.normal);
    
    vVertex.texCoord = texCoord;
    
    vTrans = transform;
    
    gl_Position = transform.P * vVertex.pos;
}
