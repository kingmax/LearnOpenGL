#pragma warning(disable: 4098)

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
using namespace std;

#include "shader.h"

// Vertex Data
// Vertex_Array_Object
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

const string vsShaderFile = "first.vs";
const string psShaderFile = "first.ps";


void framebuffer_size_callback(GLFWwindow* win, int w, int h)
{
	cout << "size changed: " << w << " x " << h << endl;
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* win)
{
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, true);
	}
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(800, 600, u8"学习OpenGL", NULL, NULL);
	if (win == NULL)
	{
		cout << "失败创建 GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

	// using Vertex Array Object (VAO) (存放VBO的数组，方便管理大量物体顶点配置数据)
	// 必须放在VBO前面
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//读入顶点数据，选产生一个VBO对象 (Vertex Buffer Object), 然后绑定到GL_ARRAY_BUFFER, 最后将实际顶点数据复制到绑定的Buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// read & compile vertex shader
	/*string _vsSource = R"(#version 330 core
layout (location = 0) in vec3 pos;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
})";
	const char* vsSource = _vsSource.c_str();
	cout << vsSource << endl;*/

	//const string _vsSource = readVS("first.vs");
	string _vsSource;
	readShaderFile(vsShaderFile, _vsSource);
	const char* vsSource = _vsSource.c_str();
	cout << vsSource << endl;

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsSource, NULL);
	glCompileShader(vertexShader);
	// check error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR:: VS Shader compile failed\n" << infoLog << endl;
	}

	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	string _psSource;
	readShaderFile(psShaderFile, _psSource);
	const char* psSource = _psSource.c_str();
	cout << psSource << endl;
	glShaderSource(fragmentShader, 1, &psSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR: PS Shader compile failed\n" << infoLog << endl;
	}

	// link vs and ps to ShaderProgramObject
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check 
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR: Link Shader Program Object failed\n" << infoLog << endl;
	}
	// clear
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// how to resolve the vertex data, 设置顶点属性指针
	// first param is layout(location =0) in vertexShader defined
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 渲染循环
	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// render..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// use ShaderProgramObject
		glUseProgram(shaderProgram);
		// 绘制数据 (通过VAO间接绑定了VBO, 而VBO已由顶点属性配置)
		glBindVertexArray(VAO);
		// 使用当前激活的shader绘制图元
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



