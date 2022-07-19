#version 330 core

struct Material {
	//vec3 ambient;
	//vec3 diffuse;
	sampler2D diffuse;
	//vec3 specular;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light {
	//vec3 position;
	vec3 direction; // FlashLight
	vec3 position;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;// = vec3(1,0.5, 0.31);
uniform vec3 lightColor;// = vec3(1);
uniform vec3 lightPos;
uniform vec3 camPos;

float ambientStrength = 0.2;
float specularStrength = 0.5;

/*void main()
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
}*/

//void main()
//{
//	// ambient
//	vec3 ambient = lightColor * material.ambient;
//	// diffuse
//	vec3 normal = normalize(Normal);
//	vec3 lightDir = normalize(lightPos - FragPos);
//	float diff = max(dot(normal, lightDir), 0.0);
//	vec3 diffuse = lightColor * (diff * material.diffuse);
//	// specular
//	vec3 viewDir = normalize(camPos - FragPos);
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//	vec3 specular = lightColor * (spec * material.specular);
//	// final color
//	vec3 result = ambient + diffuse + specular;
//	color = vec4(result, 1.0);
//}

// direction light
//void main()
//{
//	// ambient
//	//vec3 ambient = light.ambient * material.ambient;
//	vec4 diffuseMap = texture(material.diffuse, UV);
//	vec3 ambient = light.ambient * vec3(diffuseMap);
//	// diffuse
//	vec3 normal = normalize(Normal);
//	//vec3 lightDir = normalize(light.position - FragPos);
//	vec3 lightDir = normalize(-light.direction);
//	float diff = max(dot(normal, lightDir), 0.0);
//	//vec3 diffuse = light.diffuse * (diff * material.diffuse);
//	vec3 diffuse = light.diffuse * diff * vec3(diffuseMap);
//	// specular
//	vec3 viewDir = normalize(camPos - FragPos);
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//	//vec3 specular = light.specular * (spec * material.specular);
//	vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
//	// final color
//	vec3 result = ambient + diffuse + specular;
//	color = vec4(result, 1.0);
//}

void main()
{
	vec4 diffuseMap = texture(material.diffuse, UV);

	// ambient
	vec3 ambient = light.ambient * vec3(diffuseMap);

	// light attenuation
	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	
	// flash light
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	// with radius attenuation
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// diffuse
	vec3 normal = normalize(Normal);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(diffuseMap);

	// specular
	vec3 viewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));

	// final color
	/*ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;*/

	// spotLight soft edges
	diffuse *= intensity;
	specular *= intensity;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	/*if(theta > light.cutOff)
	{
		color = vec4(result, 1.0);
	}
	else
	{
		color = vec4(light.ambient * vec3(diffuseMap), 1.0);
	}*/
	color = vec4(result, 1.0);
}
