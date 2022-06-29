#version 460 core

in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 color;

void main()
{
	color = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}