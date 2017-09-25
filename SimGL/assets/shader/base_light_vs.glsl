#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

struct Transform {
    mat4 M;
    mat4 V;
    mat4 P;
} ;

struct Vertex {
    vec4 pos;
    vec3 normal;
    vec2 texCoord;
} ;

uniform Transform transform;

out Vertex vVertex;
out Transform vTrans;
out mat3 vTBN;

void main() {
    // Calculate view-space coordinate
    vVertex.pos = transform.V * transform.M * vec4(pos, 1.0);
    
    // Calculate normal in view space
    mat3 normalMatrix = mat3(transpose(inverse(transform.V * transform.M)));
    vVertex.normal = normalize(normalMatrix * normal);
    
    vec3 t = normalize(normalMatrix * tangent);
    // Orthogonalization
    t = normalize(t - dot(vVertex.normal,t)*vVertex.normal);
    
    vec3 biTangent = cross(t, vVertex.normal);
    // The tbn is belong to view space
    vTBN = mat3(t, biTangent, vVertex.normal);
    
    vVertex.texCoord = texCoord;
    
    vTrans = transform;
    
    gl_Position = transform.P * vVertex.pos;
}
