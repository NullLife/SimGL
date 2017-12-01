#version 410 core

in vec4 v_color;
in float v_liferate;

out vec4 out_color;

void main()
{
    vec4 color = v_color;
    color.a -= v_liferate;
    
    if (color.a < 0.1)
        discard;
    
    out_color = color;
}
