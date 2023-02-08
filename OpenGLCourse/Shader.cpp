#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	clear();
}

void Shader::CreateFromString(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	CompileShader(vertexShaderCode, fragmentShaderCode);
}

void Shader::CreateFromFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexString = ReadFile(vertexShaderPath);
	std::string fragmentString = ReadFile(fragmentShaderPath);

	CompileShader(vertexString.c_str(), fragmentString.c_str());
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

void Shader::use()
{
	glUseProgram(shaderID);
}

void Shader::clear()
{
	if (shaderID != 0)
	{
		glDeleteShader(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	// coping the shader code into the shader object
	// NOTE(christian): instead of passing the string length of the shader source you can just pass
	//					0 and the program assumes the string to be null terminated.
	glShaderSource(theShader, 1, &shaderCode, 0);

	// compiling the shader code
	glCompileShader(theShader);

	GLint result = 0;
	GLchar errorLog[1024] = {};

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(errorLog), 0, errorLog);
		printf("Error Compiling The %d Shader: '%s'\n", shaderType, errorLog);
		return;
	}

	// attach the shader to the program
	glAttachShader(theProgram, theShader);
}

void Shader::CompileShader(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error Creating Program!\n");
		return;
	}

	AddShader(shaderID, vertexShaderCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentShaderCode, GL_FRAGMENT_SHADER);

	// link the program
	GLint result = 0;
	GLchar errorLog[1024] = {};

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), 0, errorLog);
		printf("Error Linking Program: '%s'\n", errorLog);
		return;
	}

	// validate the program
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(errorLog), 0, errorLog);
		printf("Error Validating Program: '%s'\n", errorLog);
		return;
	}

	// get the uniform locations
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
}

std::string Shader::ReadFile(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File could not be opened.\n", filePath);
		return "";
	}

	std::string line;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}