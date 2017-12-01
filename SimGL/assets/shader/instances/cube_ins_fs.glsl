#version 410 core

uniform sampler2D tex_sampler;

in vec3 vTexCoord;
in float vA;

out vec4 color;

void main()
{
    vec4 tex_color = texture(tex_sampler, vTexCoord.xy);
    
    tex_color.a = vA;
    
    color = tex_color;
}
