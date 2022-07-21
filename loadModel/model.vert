#version 330 core

layout (location = 1) in vec3 pos;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1);
	UV = uv;
}