#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	//color = texture(ourTexture, texCoord);
	color = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);	// 贴图与顶点颜色混合
}
