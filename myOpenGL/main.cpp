#pragma warning(disable: 4098)

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
using namespace std;

#include "shader.h"
#include "util.h"
#include "shaderClass.h"

#pragma region Data
// Vertex Data
float triangle_vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

// Vertex Array Object (VAO, Array of VBO)
unsigned int VAO;

// Vertex Buffer Object (VBO)
unsigned int VBO;

// Shader File
const string vsShaderFile = "03.vert"; // "02.vert"; //"01.vert";
const string psShaderFile = "03.frag"; // "02.frag"; // "01.frag";

// Element Buffer Object (EBO)
// ������Ϊ��ɫ����
float rectangle_vertices[] = {
	0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 0.0f
};

unsigned int rectangle_indices[] = {
	0, 1, 3, // ��һ�������Σ�˳ʱ��
	1, 2, 3  // �ڶ��������Σ�˳ʱ��
};

unsigned int EBO;
#pragma endregion Data

#pragma region functions // MOVE TO util
//void framebuffer_size_callback(GLFWwindow* win, int w, int h)
//{
//	cout << "size changed: " << w << " x " << h << endl;
//	glViewport(0, 0, w, h);
//}
//
//void processInput(GLFWwindow* win)
//{
//	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(win, true);
//	}
//}
#pragma endregion functions 


int main()
{
	GLFWwindow* win;
	init(win);

	unsigned int VAO;
	VAO = prepareTriangleData(triangle_vertices);

	Shader greenShader("02.vert", "02.frag");
	Shader alphaShader("03.vert", "03.frag");
	alphaShader.setFloat("myAlphaFromCPU", 0.9f);

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// rendering..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		drawTriangle(VAO, greenShader, 3);

		
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



int main2()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// transparent ͸�� (shader::03.frag::myAlphaFromCPU)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	// �ޱ߿�
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	GLFWwindow* win = glfwCreateWindow(800, 600, u8"ѧϰOpenGL", NULL, NULL);
	if (win == NULL)
	{
		cout << "ʧ�ܴ��� GLFW window" << endl;
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

	// using Vertex Array Object (VAO) (���VBO�����飬�������������嶥����������)
	// �������VBOǰ��
	// unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//���붥�����ݣ�ѡ����һ��VBO���� (Vertex Buffer Object), Ȼ��󶨵�GL_ARRAY_BUFFER, ���ʵ�ʶ������ݸ��Ƶ��󶨵�Buffer
	// unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

#pragma region read & compile shader

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
#pragma endregion read & compile shader


	// how to resolve the vertex data, ���ö�������ָ��
	// first param is layout(location =0) in vertexShader defined
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


#pragma region use EBO
	// ʹ�� EBO �ػ�2�����������һ������ (�������̣����������ϻ����������ε�һϵ������)
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);
	////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//// ���������е���ɫ����
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
#pragma endregion use EBO

	// �߿�ģʽ
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// using shaderClass
	//Shader myShader("03.vert", "03.frag");
	Shader myShader("02.vert", "02.frag");

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// render..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();
		sendColor2Shader(myShader.ID, "ourColor");

		#pragma region draw one triangle
		// use ShaderProgramObject
		//glUseProgram(shaderProgram);
		// �������� (ͨ��VAO��Ӱ���VBO, ��VBO���ɶ�����������)
		glBindVertexArray(VAO);
		// ʹ�õ�ǰ�����shader����ͼԪ
		glDrawArrays(GL_TRIANGLES, 0, 3);
		#pragma endregion draw one triangle

		
		myShader.setFloat("myAlphaFromCPU", 0.8f);

		// using EBO draw Rectangle (2 triangles)
		//glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// send data to GPU shader (02.frag:: uniform vec4 ourColor)
		//sendColor2Shader(shaderProgram, "ourColor");

		

		glBindVertexArray(0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



