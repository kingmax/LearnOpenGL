#include "glad/glad.h"
#include "glfw3.h"

#include "../common/camera.h"
#include "../common/util.h"

#include "../common/model.h"

#include <iostream>
using namespace std;

//下面这段必须在main里面定义，主要用于init window中callback函数
const unsigned screenWidth = 800;
const unsigned screenHeight = 600;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool isFirstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main(int argc, char* argv[])
{
	/*for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}*/

	GLFWwindow* win;
	init(win, u8"学习OpenGL_LoadModel", screenWidth, screenHeight);	

	// 注意: 目录与文件名之间分隔符必须是"/", 因为：directory = path.substr(0, path.find_last_of("/"));
	//string path = R"(E:\git\learnOpenGL\loadModel\nanosuit/nanosuit.obj)";
	//string path = R"(E:\fbx\Textures/NPC_6.fbx)";
	//string path = R"(E:\fbx/Meshs/textureAtlas.fbx)";
	string path = R"(E:\fbx/Meshs/textureAtlas.obj)";
	Model theModel(path);
	theModel.ShowInfo4Debug();

	Shader modelShader("model.vert", "model.frag");
	
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	getMVP(model, view, projection, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// rendering..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		projection = glm::perspective(glm::radians(camera.Zoom), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::mat4(1.0f);
		updateMVP4Shader(modelShader, model, view, projection);

		// draw model
		theModel.Draw(modelShader);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}