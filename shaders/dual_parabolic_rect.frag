#version 330 core

uniform sampler2DRect paraboloids;

in vec3 r;

out vec4 color_out;

void main()
{
	vec3 tc = normalize(r);
	tc.xy /= abs(tc.z) + 1.0; //1
	tc.xy = tc.xy * 0.5 + 0.5; //0.5
	tc.x *= 0.5;
	tc.x += sign(-tc.z) * 0.25 + 0.25;
	
	//scale up x and y, since sampling a rectangalur texture used non-normalized coordintes, meaning they span from (0..w, 0..h).
	tc.x *= 4096;
	tc.y *= 2048;

	color_out = texture(paraboloids, tc.xy);
}
