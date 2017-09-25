#version 410 core

uniform sampler2D texSampler;

in vec2 vTexCoord;

out vec4 color;

void main() {
    color = texture(texSampler, vTexCoord);
}
