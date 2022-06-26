#include "util.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
using namespace std;

void printMaxVASupport()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
}

void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName)
{
	float _time = glfwGetTime();
	float green = sin(_time) / 2.0f;
	int variable_location = glGetUniformLocation(shaderProgram, uniformShaderVariableName);
	glUseProgram(shaderProgram);
	glUniform4f(variable_location, 0.0f, green, 0.0f, 1.0f);
}