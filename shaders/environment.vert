#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out VS_OUT 
{
	vec3 normal;
	vec3 view_dir;
} vs_out;

void main()
{
	mat4 mv_mat = view * model;
	vec4 pos_mv = mv_mat * position;

	vs_out.normal 	= mat3(mv_mat) * normal;
	vs_out.view_dir = pos_mv.xyz;

	gl_Position = proj * pos_mv;
}
