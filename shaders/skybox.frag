#version 330 core

uniform samplerCube cube_map;

in VS_OUT
{
	vec3 	tc;
} fs_in;

out vec4 color_out;

void main ()
{
	color_out = texture(cube_map, fs_in.tc);
}
