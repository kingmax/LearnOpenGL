#include "util.h"

#include <iostream>
using namespace std;


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

void init(GLFWwindow* &win)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// transparent ͸�� (shader::03.frag::myAlphaFromCPU)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	// �ޱ߿�
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//GLFWwindow* win = glfwCreateWindow(800, 600, u8"ѧϰOpenGL", NULL, NULL);
	win = glfwCreateWindow(800, 600, u8"ѧϰOpenGL", NULL, NULL);
	if (win == NULL)
	{
		cout << "ʧ�ܴ��� GLFW window" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD��ʼ��ʧ��" << endl;
		return;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

void printMaxVASupport()
{
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
}

/// <summary>
/// 
/// </summary>
/// <param name="shader">Shader Program Object</param>
/// <param name="uniformShaderVariableName"></param>
void sendColor2Shader(unsigned shader, const char* uniformShaderVariableName)
{
	float _time = (float)glfwGetTime();
	float green = sin(_time) / 2.0f;
	int variable_location = glGetUniformLocation(shader, uniformShaderVariableName);
	glUseProgram(shader);
	glUniform4f(variable_location, 0.0f, green, 0.0f, 1.0f);
}

// ����д����vertices����������������������޷������õ����ݣ������⹩�ο�����Ҫʹ�������ģ�庯������ģ�庯��Ϊ�˲��������ӱ�����Ҫ��������ʵ��д��һ���ļ���
void prepareTriangle(const float* vertices, unsigned &VAO, const unsigned stride/*=3*/)
{
	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};*/

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU�� ��CPU��������������GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
	glVertexAttribPointer(0, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
	//���ö�������
	glEnableVertexAttribArray(0); 

	//glUseProgram(shader);

	// ��VBO�洢��VAO�У��Ա�֮��ʹ�� ref: https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	/*�����������(Vertex Array Object, VAO)�����񶥵㻺������������󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С������ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ���ʹ�ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�ֻ��Ҫ�󶨲�ͬ��VAO�����ˡ��ո����õ�����״̬�����洢��VAO��*/
	// �Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬֮����VAO��֮��ʹ�á������Ǵ������һ�������ʱ������ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ�����
	// �Ȱ�VAO, ע�����VAO�Ǹ�����ֵ���������ⲿ����
	//glBindVertexArray(VAO);
}

//template<int N>
//unsigned int prepareTriangleData(float(&vertices)[N])
//{
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	//Vertex Buffer Object, ���㻺��������ڹ���GPU�Դ��еĴ�����������
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	//copy vertex data to GPU�� ��CPU��������������GPU
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	//��GPU�ṩҪ��ν�����Щ���ݣ��������������һ���ֶ�Ӧvertex shader���ĸ��������ԣ� ����������ÿ3����ʾһ��position
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //��һ������0����vert shader :: layout (location = 0)
//	//���ö�������
//	glEnableVertexAttribArray(0);
//	return VAO;
//}

// in rendering loop, ��Ҫ������Ⱦѭ����
void drawTriangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount)
{
	//glUseProgram(myShader2.ID);
	myShader.use();
	sendColor2Shader(myShader.ID, "ourColor");
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void drawRectangle(const unsigned& VAO, Shader& myShader, const unsigned vertexCount)
{
	myShader.use();
	myShader.setFloat("myAlphaFromCPU", sin(glfwGetTime()) / 2.0f + 0.5f);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTexture(const unsigned& VAO, Shader& myShader, const unsigned& texture, const unsigned vertexCount /*= 6*/)
{
	myShader.use();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTextureMix(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const unsigned vertexCount /*= 6*/)
{
	// ���������Ԫ (Ĭ���Զ��󶨵�1����GL_TEXTURE0, ���һ����16������)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	// ��Ҫָ���ĸ�sampler2D��Ӧ�ĸ�����Ԫ
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	myShader.setInt("texture2", 1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

unsigned loadTexture(const string textureFilename)
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//���������ظ������˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load texture from file
	int w, h, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(textureFilename.c_str(), &w, &h, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "ERROR:: load texture failed from <- " << textureFilename << endl;
	}
	stbi_image_free(data);

	return texture;
}
