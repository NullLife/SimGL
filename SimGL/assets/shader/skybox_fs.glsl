#version 410 core

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

in Vertex vVertex;
in Transform vTrans;

uniform sampler2D defaultSampler;

out vec4 color;

void main() {
    color = texture(defaultSampler, vVertex.texCoord);
}
