#include "Light.h"

Light::Light() :
	color(glm::vec3(1.0f, 1.0f, 1.0f)), ambientIntensity(1.0f)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity) :
	color(glm::vec3(red, green, blue)), ambientIntensity(ambientIntensity)
{
}

Light::~Light()
{
}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation)
{
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}
