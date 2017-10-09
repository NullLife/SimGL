#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;

uniform mat4 mvp_matrix;

out VS_OUT
{
    vec4 _color;
} vs_out;

void main()
{
    vs_out._color = color;
    gl_PointSize = 4.0f;
    gl_Position = mvp_matrix * vec4(pos, 1.0f);
}
