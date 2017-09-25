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
//    float depth = texture(defaultSampler, vVertex.texCoord).r;
    vec2 coord = vVertex.pos.xy * 0.5 + 0.5;
    float depth = texture(defaultSampler, coord).r;
    color = vec4(vec3(depth), 1.0);
}
