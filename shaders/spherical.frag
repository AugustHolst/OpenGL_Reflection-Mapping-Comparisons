#version 330 core

uniform sampler2D tex_env;

in VS_OUT
{
    vec3 normal;
    vec3 view_dir;
} fs_in;

out vec4 color_out;

void main() 
{
    vec3 v = normalize(fs_in.view_dir);
    
    // reflect u about the plane defined by normal at the fragment.
    //   r = v - 2 * dot(fs_in.normal, v) * fs_in.normal
    vec3 r = reflect(v, normalize(fs_in.normal));
    
    // compute scale factor
    float m = 2.0 * sqrt( pow(r.x, 2.0) + pow(r.y, 2.0) + pow(r.z+1.0, 2.0) );
    vec2 tc = r.xy / m + 0.5;
    // sample from texture using scaled and biased texture coordinate.
    color_out = texture(tex_env, tc);
}
