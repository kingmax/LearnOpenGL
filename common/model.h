#pragma once

#include "mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"

#include <string>

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	Model(const std::string &path, bool gamma = false);
	void Draw(Shader &shader);

	void ShowInfo4Debug() const;

protected:
private:
	void loadModel(const string &path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

unsigned int TextureFromFile(const string path, const string& directory, bool gamma = false);