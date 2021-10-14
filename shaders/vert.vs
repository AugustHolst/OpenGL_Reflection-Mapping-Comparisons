#version 330 core
layout (location = 0) in vec3 inPos; // the position variable has attribute position 0
layout (location = 1) in vec3 inBaseCol;
layout (location = 2) in vec2 inTexCoords;

out vec3 BaseCol;
out vec2 TexCoords;

void main()
{
    gl_Position = vec4(inPos, 1.0); // see how we directly give a vec3 to vec4's constructor
	BaseCol = inBaseCol;
	TexCoords = inTexCoords;
}