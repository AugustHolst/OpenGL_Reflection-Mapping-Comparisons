#version 330 core

uniform samplerCube cubemap;

in VS_OUT
{
	vec3 normal;
	vec3 view_dir;
} fs_in;

out vec4 color_out;

void main ()
{
	vec3 r = reflect(fs_in.view_dir, normalize(fs_in.normal));

	color_out = texture(cubemap, r);
}
