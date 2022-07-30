#version 330 core

in vec2 UV;
out vec4 color;

struct Material {
	sampler2D tex_diffuse1;
	sampler2D tex_specular1;
	float shininess;
};

uniform Material material;

uniform sampler2D tex_diffuse1;


void main()
{
	//color = texture(material.tex_diffuse1, UV);
	color = texture(tex_diffuse1, UV);
}