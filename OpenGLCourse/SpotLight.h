#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat constant, GLfloat linear, GLfloat exponent,
        GLfloat edge);

    ~SpotLight();

    // NOTE(christian): having to pass these uniform locations manually seems 
    //                  error prone. these should probably be stored in here.
    void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void setFlash(glm::vec3 position, glm::vec3 direction);

private:
    glm::vec3 direction;

    GLfloat edge, processedEdge;
};
