#version 410 core

uniform sampler2D tex_sampler;

in vec2 vTexCoord;

out vec4 color;

void main() {
    color = texture(tex_sampler, vTexCoord);
}
