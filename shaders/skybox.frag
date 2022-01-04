#version 330 core

out vec4 color_out;

in vec3 tc;

uniform samplerCube skybox;

void main()
{    
    color_out = texture(skybox, tc);
}
