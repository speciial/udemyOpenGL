#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat constant, GLfloat linear, GLfloat exponent);
    ~PointLight();

    // NOTE(christian): having to pass these uniform locations manually seems 
    //                  error prone. these should probably be stored in here.
    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

protected:
    glm::vec3 position;

    GLfloat constant;
    GLfloat linear;
    GLfloat exponent;
};

