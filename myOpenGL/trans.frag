#version 460 core

in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//color = texture(ourTexture, texCoord);
	//color = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);	// ��ͼ�붥����ɫ���
	color = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2); // ��϶�����ͼ
	color = color * vec4(ourColor, 1.0);
}
