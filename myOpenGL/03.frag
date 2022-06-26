#version 460 core

in vec3 color;	// from vert shader
out vec4 fragColor;

void main()
{
	fragColor = vec4(color, 1.0);
}