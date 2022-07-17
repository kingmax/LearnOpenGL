#version 330 core

//in vec2 texCoord;
out vec4 color;

uniform vec3 objectColor;// = vec3(1,0.5, 0.31);
uniform vec3 lightColor;// = vec3(1);

float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;

void main()
{
	//color = vec4(1, 0.5, 0.31, 1.0);
	//color = vec4(objectColor * lightColor, 1.0);
	color = vec4(ambient * objectColor, 1);
}