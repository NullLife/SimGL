#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float time;

in VS_OUT
{
    vec4 _color;
} gs_in[];

out vec4 fColor;

vec3 getNormal(vec4 pos0, vec4 pos1, vec4 pos2)
{
    vec3 a = vec3(pos0) - vec3(pos1);
    vec3 b = vec3(pos2) - vec3(pos1);
    return normalize(cross(a, b));
}

void main()
{
    fColor = gs_in[0]._color;
    gl_PointSize = gl_in[0].gl_PointSize;
    
    vec3 normal = getNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
    float magnitude = ((sin(time) + 1) / 2.0f) * 2.0f; // 使位移偏量保持在[0, 2.0f]范围内
    vec4 offset = vec4(normal * magnitude, 0.0f);
    
    gl_Position = gl_in[0].gl_Position + offset;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position + offset;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position + offset;
    EmitVertex();
    
    EndPrimitive();
}
