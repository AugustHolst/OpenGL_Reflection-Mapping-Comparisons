#version 330 core

layout (location = 0) in vec3 position;

out vec3 tc;

uniform mat4 proj;
uniform mat4 view;

void main(void)
{
    tc = mat3(proj) * mat3(view) * position;
    gl_Position = vec4(position, 1.0);
}
