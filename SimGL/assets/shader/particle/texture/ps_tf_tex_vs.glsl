#version 410 core

subroutine void PassType();
subroutine uniform PassType render_pass;

// Uniforms
uniform mat4 mvp_matrix;
uniform float time;

#define G -0.05

// Attributes
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 tex_coords;
layout(location = 2) in vec3 instance_position;
layout(location = 3) in vec3 instance_velocity;
layout(location = 4) in float instance_size;
layout(location = 5) in float instance_birth;         // s
layout(location = 6) in float instance_lifetime;      // s
layout(location = 7) in float instance_liferate;

// To transform feedback
out vec3 out_instance_position;
out vec3 out_instance_velocity;
out float out_instance_birth;
out float out_instance_liferate;

out vec3 v_tex_coords;
out float v_liferate;

float random(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

highp float random2(vec2 co){
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy ,vec2(a,b));
    highp float sn = mod(dt,3.14);
    return fract(sin(sn) * c);
}


// Update pass
subroutine (PassType) void update()
{
    float age = time - instance_birth;
    
    if (age > instance_lifetime)
    {
        float r = random2(vec2(gl_InstanceID, time));
        float ra = 6.283 * r;       // PI*2*Rand
        float rx = 2 * r * cos(ra);
        float rz = 2 * r * sin(ra);

        out_instance_position = vec3(0);

        out_instance_velocity =  vec3(rx, (6.0 * r) + 2.0, rz);

        out_instance_birth = time;
        
        out_instance_liferate = 0.0f;
    }
    else
    {
        out_instance_birth = instance_birth;
        
        out_instance_liferate = age / instance_lifetime;

        out_instance_velocity = instance_velocity; // + vec3(0, G, 0);

        out_instance_position = instance_position + 0.01 * out_instance_velocity;
    }
}

// Rendering pass
subroutine (PassType) void render()
{
    v_liferate = instance_liferate;
    
    v_tex_coords = tex_coords;
    
    vec3 tempPos = (v_liferate) * pos;
    
    gl_Position = mvp_matrix * vec4((tempPos + instance_position), 1.0);
}

void main()
{
    // Update or rendering.
    render_pass();
}
