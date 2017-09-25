#version 410 core

out vec4 color;

void main() {
    float depth = gl_FragCoord.z;
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    float depth2 = depth * depth + 0.25 * (dx*dx + dy*dy);
    color = vec4(depth, depth2, 0, 0);
}
