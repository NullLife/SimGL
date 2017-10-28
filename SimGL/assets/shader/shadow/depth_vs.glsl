#version 410 core

layout(location = 0) in vec3 pos;

struct Transform {
    mat4 M;
    mat4 V;
    mat4 P;
};

uniform Transform transform;

void main() {
    gl_Position = transform.P * transform.V * transform.M * vec4(pos,1.0);
}
