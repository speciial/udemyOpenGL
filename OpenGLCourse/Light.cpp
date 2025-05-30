#include "Light.h"

Light::Light() :
	color(glm::vec3(1.0f, 1.0f, 1.0f)), ambientIntensity(1.0f), diffuseIntensity(0.0f)
{
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity) :
	color(glm::vec3(red, green, blue)), ambientIntensity(ambientIntensity), diffuseIntensity(diffuseIntensity)
{
}

Light::~Light()
{
}
