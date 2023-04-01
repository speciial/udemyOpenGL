#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformView = 0;
	uniformProjection = 0;
	uniformEyePosition = 0;

	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformColor = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	
	uniformSpecularIntensity = 0;
	uniformShininess = 0;

	pointLightCount = 0;
	uniformPointLightCount = 0;
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

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
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
	uniformView = 0;
	uniformProjection = 0;
	uniformEyePosition = 0;

	uniformDirectionalLight.uniformAmbientIntensity = 0;
	uniformDirectionalLight.uniformColor = 0;
	uniformDirectionalLight.uniformDiffuseIntensity = 0;
	uniformDirectionalLight.uniformDirection = 0;
	
	uniformSpecularIntensity = 0;
	uniformShininess = 0;

	pointLightCount = 0;
	uniformPointLightCount = 0;
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

// NOTE(christian): this makes things so much harder to understand. where are all the parameters going?
void Shader::setDirectionalLight(DirectionalLight& directionalLight)
{
	directionalLight.useLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight* pointLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS)
	{
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t lightIndex = 0; lightIndex < lightCount; ++lightIndex)
	{
		pointLight[lightIndex].useLight(
			uniformPointLight[lightIndex].uniformAmbientIntensity,
			uniformPointLight[lightIndex].uniformColor,
			uniformPointLight[lightIndex].uniformDiffuseIntensity,
			uniformPointLight[lightIndex].uniformPosition,
			uniformPointLight[lightIndex].uniformConstant,
			uniformPointLight[lightIndex].uniformLinear,
			uniformPointLight[lightIndex].uniformExponent
		);
	}
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
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	// NOTE(christian): is this really how you would do this? can't you just use a vertex buffer for this?
	// NOTE(christian): if this works on the first try, I'll lose my mind
	for (size_t pointLightIndex = 0; pointLightIndex < MAX_POINT_LIGHTS; ++pointLightIndex)
	{
		char locBuff[100] = { '\0' };
		
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);
	
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformPosition = glGetUniformLocation(shaderID, locBuff);
		
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", pointLightIndex);
		uniformPointLight[pointLightIndex].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}
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