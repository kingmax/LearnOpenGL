#pragma once
//#ifndef SHADER_CLASS_H
//#define SHADER_CLASS_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	// shaderProgram ID
	unsigned int ID;
	Shader(const char* vertShaderFilename, const char* fragShaderFilename);
	void use();
	// help functions
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;

private:

};

//#endif // !SHADER_CLASS_H