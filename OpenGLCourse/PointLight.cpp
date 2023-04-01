#include "PointLight.h"

PointLight::PointLight() : Light(),
	position(glm::vec3(0.0f, 0.0f, 0.0f)), constant(1.0f), linear(0.0f), exponent(0.0f)
{
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat ambientIntensity, GLfloat diffuseIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat constant, GLfloat linear, GLfloat exponent) : 
	Light(red, green, blue, ambientIntensity, diffuseIntensity),
	position(glm::vec3(xPos, yPos, zPos)), constant(constant), linear(linear), exponent(exponent)
{
}

PointLight::~PointLight()
{
}

void PointLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
	GLuint diffuseIntensityLocation, GLuint positionLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}
