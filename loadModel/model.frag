#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D tex_diffuse1;

void main()
{
	color = texture(tex_diffuse1, UV);
}