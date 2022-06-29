#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	color = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2) * vec4(ourColor, 1.0);
}
