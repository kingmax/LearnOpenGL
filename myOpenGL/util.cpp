#include "util.h"

#include <iostream>
using namespace std;


void framebuffer_size_callback(GLFWwindow* win, const int w, const int h)
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

void init(GLFWwindow* &win, const int screenWidth, const int screenHeight)
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
	win = glfwCreateWindow(screenWidth, screenHeight, u8"学习OpenGL", NULL, NULL);
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

	glViewport(0, 0, screenWidth, screenHeight);
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
	// 激活、绑定纹理单元 (默认自动绑定第1个即GL_TEXTURE0, 最多一共有16个可用)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	// 需要指定哪个sampler2D对应哪个纹理单元
	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	myShader.setInt("texture2", 1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

void drawRectangleWithTextureMixTransform(const unsigned& VAO, Shader& myShader, const unsigned& texture1, const unsigned& texture2, const glm::mat4& trans, const unsigned vertexCount /*= 6*/)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	myShader.use();
	myShader.setInt("texture1", 0);
	myShader.setInt("texture2", 1);
	unsigned transformLocation = glGetUniformLocation(myShader.ID, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

unsigned loadTexture(const string textureFilename)
{
	unsigned texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//设置纹理重复、过滤方式
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

void translateTest()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 1.0f));

	vec = trans * vec;
	cout << "Vector(1,0,0) move to: (" << vec.x << "," << vec.y << "," << vec.z << ")" << endl;
}

void genTransform(glm::mat4& trans)
{
	glm::mat4 mat = glm::mat4(1.0f);
	// 实际的变换顺序应该与阅读顺序相反, 也就是说这里的矩阵做的操作实际上是：
	// 先缩放，后旋转
	mat = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0, 0, 1));
	mat = glm::scale(mat, glm::vec3(0.5, 0.5, 0.5));
	trans = mat;
}

void updateTransform(glm::mat4& trans)
{
	glm::mat4 mat = glm::mat4(1.0f);
	// 先旋转，后移动
	mat = glm::translate(mat, glm::vec3(0.5f, -0.5f, 0.0f));
	mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(0, 0, 1));
	trans = mat;
}

void getMVP(glm::mat4& model, glm::mat4& view, glm::mat4& projection, const unsigned screenWidth, const unsigned screenHeight)
{
	// 模型顶点从本地局部坐标变换到裁剪坐标(即OpenGL希望的(-1, 1)空间范围)的过程：
	// local vector (vertex position) * model matrix * view matrix * projection matrix
	// 顶点坐标变换到clip (-1,1)后，OpenGL vertex shader 将自动进行透视除法与裁剪，从而将它们变换到标准化设备坐标(Normalized Device Coordinate, NDC)
	// 之后OpenGL会使用glViewPort内部的参数来将标准化设备坐标映射到屏幕坐标，每个坐标都关联了一个屏幕上的点（在我们的例子中是一个800x600的屏幕）。这个过程称为视口变换。
	// 这样就完成了从模型顶点到屏幕像素的映射！
	// 
	// 注意：以下公式写法顺序与上面描述是相反的，因为矩阵运算的关系
	// V_clip = M_projection * M_view * M_model * V_local
	
	// model 矩阵用于将模型空间坐标变换到世界空间
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(1, 0, 0));
	// 右手坐标，Z轴指向屏幕外，将场景往-Z移动就相当于相机退后
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0, 0, -3.0f));
	// 透视投影矩阵
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
}

void updateMVP(Shader& myShader, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	unsigned modelLocation = glGetUniformLocation(myShader.ID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	unsigned viewLocation = glGetUniformLocation(myShader.ID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	unsigned projectionLocation = glGetUniformLocation(myShader.ID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}
