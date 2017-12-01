#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 texCoord;

uniform mat4 mvp_matrix;

out vec3 vTexCoord;

void main()
{
    vTexCoord = texCoord;
    gl_Position = mvp_matrix * vec4(pos, 1.0);
}
