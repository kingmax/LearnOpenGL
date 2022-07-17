#include "shaderClass.h"



Shader::Shader(const char* vertShaderFilename, const char* fragShaderFilename)
{
	string vs;
	string ps;
	ifstream vsFile;
	ifstream psFile;
	vsFile.exceptions(ifstream::failbit | ifstream::badbit);
	psFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		vsFile.open(vertShaderFilename);
		psFile.open(fragShaderFilename);
		stringstream vsSS, psSS;
		vsSS << vsFile.rdbuf();
		psSS << psFile.rdbuf();
		vsFile.close();
		psFile.close();
		vs = vsSS.str();
		ps = psSS.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER:: file not successfully read" << endl;
	}

	cout << vs << endl;
	cout << ps << endl;

	// compile shader
	const char* _vs = vs.c_str();
	const char* _ps = ps.c_str();
	unsigned vert, frag;
	int success;
	char infoLog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &_vs, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vert, 512, NULL, infoLog);
		cout << "ERROR::SHADER:: vertex shader compile failed!\n" << infoLog << endl;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &_ps, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(frag, 512, NULL, infoLog);
		cout << "ERROR::SHADER:: fragment shader compile failed!\n" << infoLog << endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vert);
	glAttachShader(ID, frag);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER:: shader program link failed!\n" << infoLog << endl;
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
}

Shader::Shader()
{
	ID = 0;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const string& name, glm::mat4 value) const
{
	int location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const string& name, const glm::vec3& value) const
{
	int location = glGetUniformLocation(ID, name.c_str());
	//glUniform3fv(location, 1, glm::value_ptr(value));
	glUniform3fv(location, 1, &value[0]);
}

void Shader::setVec3(const string& name, float a, float b, float c) const
{
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform3f(location, a, b, c);
}
