#version 410 core

uniform samplerCube tex_sampler;

in vec3 vTexCoord;

out vec4 color;

void main() {
    color = texture(tex_sampler, vTexCoord);
}
