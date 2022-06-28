#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//color = texture(ourTexture, texCoord);
	//color = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);	// 贴图与顶点颜色混合
	color = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2); // 混合二张贴图
	color = color * vec4(ourColor, 1.0);
}
