#pragma warning(disable: 4098)

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
using namespace std;

#include "shader.h"
#include "util.h"
#include "shaderClass.h"

// data
#include "data.h"

#pragma region Data
// box
extern float box_vertices[180];

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
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f
};

unsigned int rectangle_indices[] = {
	0, 1, 3, // ��һ�������Σ�˳ʱ��
	1, 2, 3  // �ڶ��������Σ�˳ʱ��
};

unsigned int EBO;

// apply texture, �����ΪUV����(���ϣ����£�����(UV����ԭ��)������)
float rectangle_vertices_with_uv[] = {
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f
};

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

const unsigned screenWidth = 800;
const unsigned screenHeight = 600;

// mouse offset handle camera movement
double lastX = screenWidth / 2;
double lastY = screenHeight / 2;
float sensitivity = 0.05f;
bool isFirstMouse = true;
float pitch = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;
glm::vec3 gCameraFrontWithMouse;
void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (isFirstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	gCameraFrontWithMouse.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	gCameraFrontWithMouse.y = sin(glm::radians(pitch));
	gCameraFrontWithMouse.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	gCameraFrontWithMouse = glm::normalize(gCameraFrontWithMouse);
}

// mouse scroll handle camera zoom in or out (FOV)
float fov = 45.0f;
void scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
	{
		fov -= yoffset;
	}
	if (fov <= 1.0f)
	{
		fov = 1.0f;
	}
	if (fov >= 45.0f)
	{
		fov = 45.0f;
	}
}

// �ع���
int main()
{
	GLFWwindow* win;
	init(win, screenWidth, screenHeight);

	// preparing data
	unsigned int VAO_Triangle;
	VAO_Triangle = prepareTriangleData(triangle_vertices);
	unsigned VAO_Rectangle;
	VAO_Rectangle = prepareRectangleData(rectangle_vertices, rectangle_indices);
	unsigned VAO_RectangleWithUV;
	VAO_RectangleWithUV = prepareRectangleWithUV(rectangle_vertices_with_uv, rectangle_indices);
	unsigned VAO_Box;
	VAO_Box = prepareBoxData(box_vertices);
	// texture
	unsigned texContainer;
	texContainer = loadTexture("container.jpg");
	unsigned texAwesomeface;
	texAwesomeface = loadTexture("awesomeface.jpg");
	// preparing shader
	Shader _01Shader("01.vert", "01.frag");
	Shader greenShader("02.vert", "02.frag");
	Shader alphaShader("03.vert", "03.frag");
	Shader uvShader("uv.vert", "uv.frag");
	Shader transShader("trans.vert", "trans.frag");
	Shader _3dShader("3d.vert", "3d.frag");
	Shader boxShader("box.vert", "box.frag");

	// math lib test
	translateTest();
	// transform
	glm::mat4 trans = glm::mat4(1.0f);
	genTransform(trans);

	// into 3D
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	getMVP(model, view, projection, screenWidth, screenHeight);

	// handle camera move
	glm::vec3 camPos = glm::vec3(0, 0, 3.0f);
	glm::vec3 camFront = glm::vec3(0, 0, -1.0f);
	glm::vec3 camUp = glm::vec3(0, 1, 0);
	const float speed = 0.5f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	// with mouse move handle camera
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win, mouse_callback);
	// mouse scroll handle camera fov
	glfwSetScrollCallback(win, scroll_callback);

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// rendering..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// ��סR��ʱ��Ⱦ����
		if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
		{
			//cout << "R Key Pressed, switch to render Rectangle" << endl;
			drawRectangle(VAO_Rectangle, alphaShader, 6);
		}
		else if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
		{
			drawRectangleWithTexture(VAO_RectangleWithUV, uvShader, texContainer, 6);
		}
		else if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS)
		{
			drawRectangleWithTextureMix(VAO_RectangleWithUV, uvShader, texContainer, texAwesomeface, 6);
		}
		else if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS)
		{
			drawRectangleWithTextureMixTransform(VAO_RectangleWithUV, transShader, texContainer, texAwesomeface, trans, 6);
			updateTransform(trans);
		}
		else if (glfwGetKey(win, GLFW_KEY_5) == GLFW_PRESS)
		{
			updateMVP4Shader(_3dShader, model, view, projection);
			drawRectangleWithTextureMix(VAO_RectangleWithUV, _3dShader, texContainer, texAwesomeface, 6);
		}
		else if (glfwGetKey(win, GLFW_KEY_6) == GLFW_PRESS)
		{
			// transform animation
			model = glm::rotate(model, 0.1f * (float)sin(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0));
			updateMVP4Shader(_3dShader, model, view, projection);
			drawRectangleWithTextureMix(VAO_RectangleWithUV, _3dShader, texContainer, texAwesomeface, 6);
		}
		else if (glfwGetKey(win, GLFW_KEY_7) == GLFW_PRESS)
		{
			model = glm::rotate(model, 0.1f * (float)sin(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0));
			updateMVP4Shader(boxShader, model, view, projection);
			drawBox(VAO_Box, boxShader, texContainer, texAwesomeface, 180);
		}
		else if (glfwGetKey(win, GLFW_KEY_8) == GLFW_PRESS)
		{
			updateMVP4Shader(boxShader, model, view, projection);
			drawBox10(VAO_Box, boxShader, texContainer, texAwesomeface, 180);
		}
		else if (glfwGetKey(win, GLFW_KEY_9) == GLFW_PRESS)
		{
			view = getAnimateView(10.0f);
			updateMVP4Shader(boxShader, model, view, projection);
			drawBox10(VAO_Box, boxShader, texContainer, texAwesomeface, 180);
		}
		else if (glfwGetKey(win, GLFW_KEY_0) == GLFW_PRESS)
		{
			// ��Ⱦ������
			drawTriangle(VAO_Triangle, greenShader, 3);
			//drawTriangle(VAO_Triangle, _01Shader, 3);
		}
		else 
		{
			//drawTriangle(VAO_Triangle, greenShader, 3);
			//drawRectangle(VAO_Rectangle, alphaShader, 6);
			//drawRectangleWithTexture(VAO_RectangleWithUV, uvShader, texContainer, 6);
			//drawRectangleWithTexture(VAO_RectangleWithUV, uvShader, texAwesomeface, 6);
			//drawRectangleWithTextureMix(VAO_RectangleWithUV, uvShader, texContainer, texAwesomeface, 6);
			// transform animation
			//drawRectangleWithTextureMixTransform(VAO_RectangleWithUV, transShader, texContainer, texAwesomeface, trans, 6);
			//updateTransform(trans);

			/*model = glm::rotate(model, 0.1f * (float)sin(glfwGetTime()), glm::vec3(0.5f, 0.5f, 0));
			updateMVP4Shader(boxShader, model, view, projection);
			drawBox(VAO_Box, boxShader, texContainer, texAwesomeface, 180);*/

			// handle camera
			getDeltaTime(deltaTime, lastTime);
			//view = handleCameraView(win, camPos, camFront, camUp, speed);
			//view = handleCameraView(win, camPos, camFront, camUp, speed * deltaTime);
			// mouse move handle camFront
			view = handleCameraView(win, camPos, gCameraFrontWithMouse, camUp, speed * deltaTime);
			updateMVP4Shader(boxShader, model, view, projection);
			drawBox10(VAO_Box, boxShader, texContainer, texAwesomeface, 180);
			// mouse scroll handle camera fov zoom in or zoom out
			projection = glm::perspective(glm::radians(fov), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
		}

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


// δ�ع�ǰ��
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
	//unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//���붥�����ݣ�ѡ����һ��VBO���� (Vertex Buffer Object), Ȼ��󶨵�GL_ARRAY_BUFFER, ���ʵ�ʶ������ݸ��Ƶ��󶨵�Buffer
	//unsigned int VBO;
	glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

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
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);


#pragma region use EBO
	// ʹ�� EBO �ػ�2�����������һ������ (�������̣����������ϻ����������ε�һϵ������)
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangle_indices), rectangle_indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���������е���ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion use EBO

	// �߿�ģʽ
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// using shaderClass
	Shader myShader("03.vert", "03.frag");
	//Shader myShader("02.vert", "02.frag");

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// render..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.use();
		//sendColor2Shader(myShader.ID, "ourColor");

		#pragma region draw one triangle
		// use ShaderProgramObject
		//glUseProgram(shaderProgram);
		// �������� (ͨ��VAO��Ӱ���VBO, ��VBO���ɶ�����������)
		//glBindVertexArray(VAO);
		// ʹ�õ�ǰ�����shader����ͼԪ
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		#pragma endregion draw one triangle

		
		myShader.setFloat("myAlphaFromCPU", 0.8f);

		// using EBO draw Rectangle (2 triangles)
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// send data to GPU shader (02.frag:: uniform vec4 ourColor)
		//sendColor2Shader(shaderProgram, "ourColor");

		

		glBindVertexArray(0);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}



