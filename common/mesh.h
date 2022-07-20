#pragma once

#include <vector>
#include "shaderClass.h"

struct Vertex;
struct Texture;

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	/// <summary>
	/// 注意shader中贴图命名规范必须是这样形式: tex_{textureType}{N}
	/// 其中textureType为diffuse或specular, N为数字1,2,3...
	/// 例如: uniform sampler2D tex_diffuse1; uniform sampler2D tex_specular1
	/// </summary>
	/// <param name="shader"></param>
	void Draw(Shader shader);
protected:
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};