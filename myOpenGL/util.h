#pragma once

#include <glad/glad.h>
#include <glfw3.h>
// texture loader
#include "stb_image.h"
// math lib
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// data
#include "data.h"
// shader
#include "shaderClass.h"

#pragma region window
void framebuffer_size_callback(GLFWwindow* win, const int w, const int h);
void processInput(GLFWwindow* win);

void init(GLFWwindow*& win, const int screenWidth=800, const int screenHeight=600);

void printMaxVASupport();
#pragma endregion window


// 从程序代码(CPU端)给Shader全局变量提供数据 (02.frag中的uniform vec4 ourColor;)
void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName);


#pragma region PrepareData and DrawModel

// 错误写法，vertices数组根本传不进来，所以无法正常得到数据，留在这供参考，需要使用下面的模板函数，而模板函数为了不发生链接报错，需要将定义与实现写在一个文件中
void prepareTriangle(const float* vertices, unsigned& VAO, const unsigned stride=3);

// 因为模板函数编译时不生成实际代码，所以如果不把实现放在同一个文件中，在main中调用，链接时时就会出现报错:"无法解析的外部符号"
// https://blog.csdn.net/qq_39779233/article/details/102973324
// 数组引用作为函数参数，必须象下面这样写，否则传数组作为参数都将视为指针，而仅一个指针无法确定数组长度
// https://bbs.csdn.net/topics/390362450
template<int N>
unsigned int prepareTriangleData(float(&vertices)[N])
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, 顶点缓冲对象，用于管理GPU显存中的大量顶点数据
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU， 从CPU将顶点数据送入GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//向GPU提供要如何解释这些数据，即输入的数据哪一部分对应vertex shader的哪个顶点属性， 如上述数据每3个表示一个position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //第一个参数0来自vert shader :: layout (location = 0)
	//启用顶点属性
	glEnableVertexAttribArray(0);
	return VAO;
}

// 默认参数需要写在头文件中
void drawTriangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount = 3);


template<int M, int N>
unsigned int prepareRectangleData(float(&vertices)[M], unsigned int(&indices)[N], const unsigned stride = 6)
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

void drawRectangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount=6);

template<int M, int N>
unsigned int prepareRectangleWithUV(float(&vertices)[M], unsigned int(&indices)[N], const unsigned stride = 8)
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return VAO;
}


void drawRectangleWithTexture(const unsigned& VAO, Shader& myShader, const unsigned& texture, const unsigned vertexCount = 6);
void drawRectangleWithTextureMix(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 6);
void drawRectangleWithTextureMixTransform(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4& trans, const unsigned vertexCount = 6);

template<int N>
unsigned int prepareBoxData(float(&vertices)[N])
{
	// VBO Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//describe to shader
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	return VAO;
}

void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 180);
// draw one box with position, ratation
void drawBox(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4 model, const unsigned vertexCount = 180);

glm::mat4 getNewModelMatrix(glm::vec3& position);

void drawBox10(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount = 180);
#pragma endregion PrepareData and DrawModel


#pragma region Texture
unsigned loadTexture(const string textureFilename);
#pragma endregion Texture


#pragma region Transformation
void translateTest();
void genTransform(glm::mat4& trans);
void updateTransform(glm::mat4& trans);

// welcome to 3D
void getMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const unsigned screenWidth, const unsigned screenHeight);
// send MVP matrix to vertex shader (3d.vert)
void updateMVP4Shader(Shader& myShader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

// camera (view Matrix)
glm::mat4 getAnimateView(const float radius = 10.0f);

#pragma endregion Transformation