#include "glad/glad.h"
#include "glfw3.h"

#include "util.h"

#include <iostream>
using namespace std;

extern float box_vertices[180];


int main(int argc, char* argv[])
{
	/*cout << argc << endl;
	for (int i=0; i<argc; i++)
	{
		cout << argv[i] << endl;
	}*/
	const unsigned screenWidth = 800;
	const unsigned screenHeight = 600;

	GLFWwindow* win;
	init(win, screenWidth, screenHeight);

	unsigned vao_box = prepareBoxData(box_vertices);
	unsigned vao_light = prepareBoxData(box_vertices);

	Shader boxLightingShader("boxLighting.vert", "boxLighting.frag");
	//boxLightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	//boxLightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	//boxLightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	//glUniform3fv(glGetUniformLocation(boxLightingShader.ID, "objectColor"), 1, &objectColor[0]);
	glUniform3f(glGetUniformLocation(boxLightingShader.ID, "objectColor"), 1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	//glUniform3fv(glGetUniformLocation(boxLightingShader.ID, "lightColor"), 1, &lightColor[0]);
	glUniform3f(glGetUniformLocation(boxLightingShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f);
	//boxLightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	//Shader lightShader("boxLighting.vert", "light.frag");

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	getMVP(model, view, projection, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(win))
	{
		processInput(win);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		updateMVP4Shader(boxLightingShader, model, view, projection);
		drawBox(vao_box, boxLightingShader, 180);
		//updateMVP4Shader(lightShader, model, view, projection);
		//drawBox(vao_light, lightShader, 180);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}