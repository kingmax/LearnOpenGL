#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;// = vec3(1,0.5, 0.31);
uniform vec3 lightColor;// = vec3(1);
uniform vec3 lightPos;

float ambientStrength = 0.1;
vec3 ambient = ambientStrength * lightColor;



void main()
{
	//color = vec4(1, 0.5, 0.31, 1.0);
	//color = vec4(objectColor * lightColor, 1.0);
	//color = vec4(ambient * objectColor, 1);

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuse = max(dot(normal, lightDir), 0.0);
	vec3 diffuseColor = diffuse * lightColor;
	vec3 result = (ambient + diffuseColor) * objectColor;
	color = vec4(result, 1.0);
}