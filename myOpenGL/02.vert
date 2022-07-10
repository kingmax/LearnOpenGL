#version 330 core

layout (location = 0)
in vec3 pos;
out vec4 color; // send to fragment shader

void main()
{
	gl_Position = vec4(pos, 1.0);
	//color = vec4(0.5, 0, 0, 1.0);
	color = vec4(pos, 1.0);
}
