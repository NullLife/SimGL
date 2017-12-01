#version 410 core

uniform sampler2D tex_sampler;

in vec3 vTexCoord;

out vec4 color;

void main()
{
    color = texture(tex_sampler, vTexCoord.xy);
}
