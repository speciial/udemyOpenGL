#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir);

    ~DirectionalLight();

    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
    glm::vec3 direction;
};

