#include "Light.h"

Light::Light() :
	color(glm::vec3(1.0f, 1.0f, 1.0f)), ambientIntensity(1.0f),
	direction(glm::vec3(0.0f, -1.0f, 0.0f)), diffuseIntensity(0.0f)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity) :
	color(glm::vec3(red, green, blue)), ambientIntensity(ambientIntensity),
	direction(glm::vec3(xDir, yDir, zDir)), diffuseIntensity(diffuseIntensity)
{
}

Light::~Light()
{
}

void Light::useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);

	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);

}
