#version 330 core

in vec3 color;	// from vert shader
out vec4 fragColor;

uniform float myAlphaFromCPU;

void main()
{
	//fragColor = vec4(color, 1.0);
	fragColor = vec4(color, myAlphaFromCPU);
}