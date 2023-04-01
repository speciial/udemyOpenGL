#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(),
	direction(glm::vec3(0.0f, -1.0f, 0.0f))
{
}

// NOTE(christian): this is getting really hard to read. maybe don't use the initializer lists?
DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat ambientIntensity, GLfloat diffuseIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : 
	Light(red, green, blue, ambientIntensity, diffuseIntensity),
	direction(glm::vec3(xDir, yDir, zDir))
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);

}
