#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 texCoord;
layout(location = 2) in vec3 instance_pos;

uniform mat4 mvp_matrix;

out vec3 vTexCoord;
out float vA;

void main()
{
    vTexCoord = texCoord;
    
    if (gl_InstanceID < 24)
        vA = 0.5f;
    else
        vA = 1.0f;
    
    vec3 newpos = pos + instance_pos;
    
    gl_Position = mvp_matrix * vec4(newpos, 1.0);
}
