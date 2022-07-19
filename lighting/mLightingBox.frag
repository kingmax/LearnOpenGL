#version 330 core

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 camPos;

out vec4 color;


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

	sampler2D castMap;
};
uniform Material material;

// direction light, eg. sun
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 diffuseMap = vec3(texture(material.diffuse, UV));
	vec3 specularMap = vec3(texture(material.specular, UV));

	vec3 ambient = light.ambient * diffuseMap;
	vec3 diffuse = light.diffuse * diff * diffuseMap;
	vec3 specular = light.specular * spec * specularMap;

	return ambient + diffuse + specular;
}

// point light
struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define N_POINT_LIGHTS 4
uniform PointLight pointLights[N_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	vec3 diffuseMap = vec3(texture(material.diffuse, UV));
	vec3 specularMap = vec3(texture(material.specular, UV));

	vec3 ambient = light.ambient * diffuseMap * attenuation;
	vec3 diffuse = light.diffuse * diff * diffuseMap * attenuation;
	vec3 specular = light.specular * spec * specularMap * attenuation;

	return ambient + diffuse + specular; 
}

// spot light, eg. flashLight
struct SpotLight {
	vec3 direction;
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
uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	vec3 diffuseMap = vec3(texture(material.diffuse, UV));
	vec3 specularMap = vec3(texture(material.specular, UV));
	// cast texture map
	vec3 castMap = vec3(texture(material.castMap, UV));

	vec3 ambient = light.ambient * diffuseMap;
	vec3 diffuse = light.diffuse * diff * (diffuseMap + castMap) * intensity;
	vec3 specular = light.specular * spec * specularMap * intensity;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular; 
}


void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(camPos - FragPos);

	vec3 result = vec3(0.0);

	result = CalcDirLight(dirLight, normal, viewDir);
	for(int i = 0; i < N_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
	}
	result += CalcSpotLight(spotLight, normal, FragPos, viewDir);

	color = vec4(result, 1.0);
}



