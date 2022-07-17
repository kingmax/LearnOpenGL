#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;// = vec3(1,0.5, 0.31);
uniform vec3 lightColor;// = vec3(1);
uniform vec3 lightPos;
uniform vec3 camPos;

float ambientStrength = 0.2;
vec3 ambient = ambientStrength * lightColor;

float specularStrength = 0.5;

void main()
{
	//color = vec4(1, 0.5, 0.31, 1.0);
	//color = vec4(objectColor * lightColor, 1.0);
	//color = vec4(ambient * objectColor, 1);

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0);
}