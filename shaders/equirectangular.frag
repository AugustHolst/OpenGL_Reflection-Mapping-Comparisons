#version 330 core

uniform sampler2D tex_env;

in VS_OUT
{
	vec3 normal;
	vec3 view_dir;
} fs_in;

out vec4 color_out;

void main ()
{
	vec3 v = normalize(fs_in.view_dir);

	vec3 r = reflect(v, normalize(fs_in.normal));

	vec2 tc; // texture coordinate.

	tc.y = r.y; // set y to reflection.y, essentially the altitude. 
	r.y = 0; // project reflection vector onto xy-plane
	tc.x = normalize(r).x * 0.5; // set x to what is effectively the azimuth angle.

	float s = sign(r.z) * 0.5;
	
	tc.s = 0.75 - s * (0.5 - tc.s);
	tc.t = 0.5 + 0.5 * tc.t;

	color_out = texture(tex_env, tc);
}
