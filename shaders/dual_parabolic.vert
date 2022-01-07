#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 cam_pos;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 r;

void main()
{
	mat4 mv_mat = view * model;

	vec3 world_pos = mat3(mv_mat) * position.xyz;
	vec3 world_norm = mat3(mv_mat) * normal;
	vec3 view_dir = normalize(world_pos - cam_pos);
	r = reflect(view_dir, normalize(world_norm));
	gl_Position = vec4(world_pos, 1.0);
}
