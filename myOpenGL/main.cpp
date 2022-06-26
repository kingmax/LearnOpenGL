#pragma warning(disable: 4098)

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
using namespace std;

#include "shader.h"
#include "util.h"

#pragma region Data
// Vertex Data
float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

// Vertex Array Object (VAO, Array of VBO)
unsigned int VAO;

// Vertex Buffer Object (VBO)
unsigned int VBO;

// Shader File
const string vsShaderFile = "02.vert"; //"01.vert";
const string psShaderFile = "02.frag"; // "01.frag";

// Element Buffer Object (EBO)
float rectangle_vertices[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

unsigned int rectangle_indices[] = {
	0, 1, 3, // 第一个三角形，顺时针
	1, 2, 3  // 第二个三角形，顺时针
};

unsigned int EBO;
#pragma endregion Data



#pragma region functions
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
#pragma endregion functions


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

	printMaxVASupport();

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

	// using Vertex Array Object (VAO) (存放VBO的数组，方便管理大量物体顶点配置数据)
	// 必须放在VBO前面
	// unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//读入顶点数据，选产生一个VBO对象 (Vertex Buffer Object), 然后绑定到GL_ARRAY_BUFFER, 最后将实际顶点数据复制到绑定的Buffer
	// unsigned int VBO;
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

	// 使用 EBO 重画2个三角形组成一个矩形 (完整流程，覆盖了以上画单个三角形的一系列设置)
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 线框模式
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 渲染循环
	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// render..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		#pragma region draw one triangle
		// use ShaderProgramObject
		//glUseProgram(shaderProgram);
		// 绘制数据 (通过VAO间接绑定了VBO, 而VBO已由顶点属性配置)
		//glBindVertexArray(VAO);
		// 使用当前激活的shader绘制图元
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		#pragma endregion draw one triangle

		// using EBO draw Rectangle (2 triangles)
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// send data to GPU shader (02.frag:: uniform vec4 ourColor)
		sendColor2Shader(shaderProgram, "ourColor");

		glBindVertexArray(0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



