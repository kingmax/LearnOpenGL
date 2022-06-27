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

	// transparent 透明 (shader::03.frag::myAlphaFromCPU)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	// 无边框
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//GLFWwindow* win = glfwCreateWindow(800, 600, u8"学习OpenGL", NULL, NULL);
	win = glfwCreateWindow(800, 600, u8"学习OpenGL", NULL, NULL);
	if (win == NULL)
	{
		cout << "失败创建 GLFW window" << endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD初始化失败" << endl;
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

// 错误写法，vertices数组根本传不进来，所以无法正常得到数据，留在这供参考，需要使用下面的模板函数，而模板函数为了不发生链接报错，需要将定义与实现写在一个文件中
void prepareTriangle(const float* vertices, unsigned &VAO, const unsigned stride/*=3*/)
{
	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};*/

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex Buffer Object, 顶点缓冲对象，用于管理GPU显存中的大量顶点数据
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy vertex data to GPU， 从CPU将顶点数据送入GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//向GPU提供要如何解释这些数据，即输入的数据哪一部分对应vertex shader的哪个顶点属性， 如上述数据每3个表示一个position
	glVertexAttribPointer(0, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); //第一个参数0来自vert shader :: layout (location = 0)
	//启用顶点属性
	glEnableVertexAttribArray(0); 

	//glUseProgram(shader);

	// 将VBO存储在VAO中，以备之后使用 ref: https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
	/*顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的VAO就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的VAO就行了。刚刚设置的所有状态都将存储在VAO中*/
	// 从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。当我们打算绘制一个物体的时候，我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了
	// 先绑定VAO, 注意这个VAO是个引用值，将返回外部调用
	//glBindVertexArray(VAO);
}

//template<int N>
//unsigned int prepareTriangleData(float(&vertices)[N])
//{
//	unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	//Vertex Buffer Object, 顶点缓冲对象，用于管理GPU显存中的大量顶点数据
//	unsigned int VBO;
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	//copy vertex data to GPU， 从CPU将顶点数据送入GPU
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	//向GPU提供要如何解释这些数据，即输入的数据哪一部分对应vertex shader的哪个顶点属性， 如上述数据每3个表示一个position
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //第一个参数0来自vert shader :: layout (location = 0)
//	//启用顶点属性
//	glEnableVertexAttribArray(0);
//	return VAO;
//}

// in rendering loop, 需要放在渲染循环中
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
	myShader.setFloat("myAlphaFromCPU", 0.9f);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}