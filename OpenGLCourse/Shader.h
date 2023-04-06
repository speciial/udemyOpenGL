#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

// NOTE(christian): the shader should not depend on the light classes.
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "CommonValues.h"

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
	GLuint GetEyePositionLocation();

	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();

	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();

	void setDirectionalLight(DirectionalLight& directionalLight);
	void setPointLights(PointLight* pointLight, unsigned int lightCount);
	void setSpotLights(SpotLight* spotLight, unsigned int lightCount);

	void use();
	void clear();
private:
	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformEyePosition;

	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	int pointLightCount;
	GLuint uniformPointLightCount;
	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	int spotLightCount;
	GLuint uniformSpotLightCount;
	struct
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	void CompileShader(const char* vertexShaderCode, const char* fragmentShaderCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	std::string ReadFile(const char* filePath);
};

