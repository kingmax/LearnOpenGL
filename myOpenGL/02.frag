#version 460 core

in vec4 color; // get from vertex shader
out vec4 fragColor;

uniform vec4 ourColor; // get from OpenGL Program (CPU side!)

void main()
{
	//fragColor = color;
	fragColor = ourColor;
}
