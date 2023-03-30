#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode);
	void CreateFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);
	
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	
	void use();
	void clear();
private:
	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;

	GLuint uniformAmbientIntensity;
	GLuint uniformAmbientColor;

	void CompileShader(const char* vertexShaderCode, const char* fragmentShaderCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string ReadFile(const char* filePath);
};

