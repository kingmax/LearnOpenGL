#include "glad/glad.h"
#include "glfw3.h"

#include "../common/camera.h"
#include "../common/util.h"

#include <iostream>
using namespace std;

//������α�����main���涨�壬��Ҫ����init window��callback����
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
	init(win, u8"ѧϰOpenGL_LoadModel", screenWidth, screenHeight);	

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);

		// rendering..
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}