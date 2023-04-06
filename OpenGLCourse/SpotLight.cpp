#include "SpotLight.h"

SpotLight::SpotLight() : PointLight(),
	direction(glm::vec3(0.0f, -1.0f, 0.0f)), edge(0)
{
	processedEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat ambientIntensity, GLfloat diffuseIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat constant, GLfloat linear, GLfloat exponent,
	GLfloat edge) : PointLight(red, green, blue, 
		ambientIntensity, diffuseIntensity,
		xPos, yPos, zPos, constant, linear, exponent),
	direction(glm::normalize(glm::vec3(xDir, yDir, zDir))), edge(edge)
{
	processedEdge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight()
{
}

void SpotLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(glm::vec3 position, glm::vec3 direction)
{
	this->position = position;
	this->direction = direction;
}
