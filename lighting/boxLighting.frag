#version 460 core

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	color = vec4(objectColor * lightColor, 1.0);
	//color = vec4(1, 0.5, 0.31, 1.0);
}