#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoord;

uniform mat4 mvp_matrix;

out vec3 vTexCoord;

void main() {
    vTexCoord = pos;
    gl_Position = mvp_matrix * vec4(pos, 1.0);
}
