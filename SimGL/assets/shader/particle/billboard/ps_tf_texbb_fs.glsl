#version 410 core

uniform sampler2D tex_sampler;

in vec3 v_tex_coords;
in float v_liferate;

out vec4 out_color;

void main()
{
//    //Distance from center
//    vec2 delta = v_tex_coords.xy - vec2(0.5, 0.5);
//
//    //Length Squared (avoiding square roots)
//    float lenSqr = abs(dot(delta, delta));
//
//    //Center, so 0.5*0.5 = 0.25, the squared len from center, avoiding roots.
//    float a = smoothstep(0.25, 0.24, lenSqr); // if lenSqr < 0.25 => a > 0  else => a = 0
//
//    //Whatever the smooth step is, set as alpha blend.
//    vec4 color = vec4(a, 0.0, 0.0, a);
    vec4 color = texture(tex_sampler, v_tex_coords.xy);

//    color.a -= v_liferate;

//    if (color.a < 0.1)
//        discard;

    out_color = color;
}
