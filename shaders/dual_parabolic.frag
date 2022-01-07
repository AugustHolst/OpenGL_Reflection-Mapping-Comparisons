#version 330 core

uniform sampler1DArray paraboloids;

uniform sampler2D front_tex;
uniform sampler2D back_tex;

in vec3 r;

out vec4 color_out;

void main (void)
{
	vec4 output_color;
	
	vec3 vR = normalize(r);
	
	// Select the front or back env map according to the sign of vR.z.
	if(vR.z>0.0)
	{
		// calculate the forward paraboloid map texture coordinates	
		vec2 frontUV;
		frontUV = (vR.xy / (2.0*(1.0 + vR.z))) + 0.5;
		output_color = texture(paraboloids, frontUV );
	}
	else
	{
		// calculate the backward paraboloid map texture coordinates
		vec2 backUV;
		backUV = (vR.xy / (2.0*(1.0 - vR.z))) + 0.5;
		output_color = texture(paraboloids, backUV );
	}
	
	color_out = output_color;
}
