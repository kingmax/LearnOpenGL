#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include "shaderClass.h"

void framebuffer_size_callback(GLFWwindow* win, int w, int h);
void processInput(GLFWwindow* win);

void init(GLFWwindow*& win);

void printMaxVASupport();

// �ӳ������(CPU��)��Shaderȫ�ֱ����ṩ���� (02.frag�е�uniform vec4 ourColor;)
void sendColor2Shader(unsigned shaderProgram, const char* uniformShaderVariableName);

// ����д����vertices����������������������޷������õ����ݣ������⹩�ο�����Ҫʹ�������ģ�庯������ģ�庯��Ϊ�˲��������ӱ�����Ҫ��������ʵ��д��һ���ļ���
void prepareTriangle(const float* vertices, unsigned& VAO, const unsigned stride=3);

// ��Ϊģ�庯������ʱ������ʵ�ʴ��룬�����������ʵ�ַ���ͬһ���ļ��У���main�е��ã�����ʱʱ�ͻ���ֱ���:"�޷��������ⲿ����"
// https://blog.csdn.net/qq_39779233/article/details/102973324
// ����������Ϊ������������������������д������������Ϊ����������Ϊָ�룬����һ��ָ���޷�ȷ�����鳤��
// https://bbs.csdn.net/topics/390362450
template<int N>
unsigned int prepareTriangleData(float(&vertices)[N])
{
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU�� ��CPU��������������GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
	//���ö�������
	glEnableVertexAttribArray(0);
	return VAO;
}

// Ĭ�ϲ�����Ҫд��ͷ�ļ���
void drawTriangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount = 3);


template<int M, int N>
unsigned int prepareRectangleData(float(&vertices)[M], unsigned int(&indices)[N])
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}

void drawRectangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount=6);

