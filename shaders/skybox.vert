#version 330 core

uniform mat4 view_mat;

out VS_out
{
	vec3 	tc;
} vs_out;

void main() 
{
	vec3[4] verts = vec3[4](vec3(-1.0, -1.0, 1.0),
							vec3( 1.0, -1.0, 1.0),
							vec3(-1.0,  1.0, 1.0),
							vec3( 1.0,  1.0, 1.0));

	vs_out.tc = mat3(view_mat) * verts[gl_VertexID];
	gl_Position = vec4(verts[gl_VertexID], 1.0);
}
