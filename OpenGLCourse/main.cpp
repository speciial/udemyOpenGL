#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

// NOTE(christian): understand the phong lighting model in more detail. 

// window dimensions
const GLint WIDTH = 1280;
const GLint HEIGHT = 720;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// shader file paths
const char* vertexShaderPath = "shaders/shader.vert";
const char* fragmentShaderPath = "shaders/shader.frag";

void calcAverageNormals(GLuint *indices, unsigned int indexCount, 
	GLfloat *vertices, unsigned int vertexCount,
	unsigned int vLength, unsigned int normalOffset)
{
	// TODO(christian): what is going on here?
	for (size_t triangleVertIndex = 0; triangleVertIndex < indexCount; triangleVertIndex += 3)
	{
		unsigned int triIndex0 = indices[triangleVertIndex] * vLength;
		unsigned int triIndex1 = indices[triangleVertIndex + 1] * vLength;
		unsigned int triIndex2 = indices[triangleVertIndex + 2] * vLength;

		glm::vec3 v1(
			vertices[triIndex1] - vertices[triIndex0],
			vertices[triIndex1 + 1] - vertices[triIndex0 + 1],
			vertices[triIndex1 + 2] - vertices[triIndex0 + 2]);
		glm::vec3 v2(
			vertices[triIndex2] - vertices[triIndex0],
			vertices[triIndex2 + 1] - vertices[triIndex0 + 1],
			vertices[triIndex2 + 2] - vertices[triIndex0 + 2]);
	
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		triIndex0 += normalOffset;
		triIndex1 += normalOffset;
		triIndex2 += normalOffset;
		vertices[triIndex0] += normal.x; vertices[triIndex0 + 1] += normal.y; vertices[triIndex0 + 2] += normal.z;
		vertices[triIndex1] += normal.x; vertices[triIndex1 + 1] += normal.y; vertices[triIndex1 + 2] += normal.z;
		vertices[triIndex2] += normal.x; vertices[triIndex2 + 1] += normal.y; vertices[triIndex2 + 2] += normal.z;
	}

	for (size_t vertexIndex = 0; vertexIndex < (vertexCount / vLength); ++vertexIndex)
	{
		unsigned int nOffset = (vertexIndex * vLength) + normalOffset;
		glm::vec3 vec = glm::normalize(
			glm::vec3(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]));
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

// NOTE(christian): this is probably pretty bad code. Since it is simple it won't hurt 
//					to have this here, but the creation of objects should be handled 
//					differently.
void CreateObjects()
{
	GLuint indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		// x      y      z      u     v     nx    ny    nz
		-1.0f, -1.0f, -0.6f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
	};

	GLuint floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] =
	{
		-10.0f, 0.0f, -10.0f,   0.0f,  0.0f,  0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,  10.0f,  0.0f,  0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f,   0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,  10.0f, 10.0f,  0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	// NOTE(christian): I still don't understand why there are "create" calls instead of just
	//					using the contructor. 
	Mesh* mesh1 = new Mesh();
	mesh1->Create(vertices, indices, 32, 12);
	meshList.push_back(mesh1);

	Mesh* mesh2 = new Mesh();
	mesh2->Create(vertices, indices, 32, 12);
	meshList.push_back(mesh2);

	Mesh* floorMesh = new Mesh();
	floorMesh->Create(floorVertices, floorIndices, 32, 6);
	meshList.push_back(floorMesh);
}

void CreateShaders()
{
	Shader* shader = new Shader();
	shader->CreateFromFile(vertexShaderPath, fragmentShaderPath);
	shaderList.push_back(shader);
}

int main()
{
	Window mainWindow(WIDTH, HEIGHT);
	mainWindow.initialize();

	// set up viewport size
	glViewport(0, 0, mainWindow.getWidth(), mainWindow.getHeight());

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// set up triangle and shaders
	CreateObjects();
	CreateShaders();

	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

	Texture brickTexture("textures/brick.png");
	brickTexture.LoadTexture();
	Texture dirtTexture("textures/dirt.png");
	dirtTexture.LoadTexture();
	Texture plainTexture("textures/plain.png");
	plainTexture.LoadTexture();

	Material shinyMaterial(1.0f, 32.0f);
	Material dullMaterial(0.3f, 4.0f);

	DirectionalLight mainLight(1.0f, 1.0f, 1.0f, 
							   0.0f, 0.0f, 
							   2.0f, -1.0f, -2.0f);

	unsigned int pointLightCount = 0;
	PointLight pointLights[MAX_POINT_LIGHTS] = {};
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f, 
								0.1f, 0.4f,
								4.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	++pointLightCount;
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.1f, 0.5f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	++pointLightCount;

	pointLightCount = 0;

	unsigned int spotLightCount = 0;
	SpotLight spotLights[MAX_SPOT_LIGHTS] = {};
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.1f, 1.0f,
							  0.0f, 0.0f, 0.0f,
							  0.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  30.0f);
	++spotLightCount;
	spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
							  0.0f, 1.0f,
							  0.0f, -1.5f, 0.0f,
							  -100.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  20.0f);
	++spotLightCount;

	// NOTE(christian): I understand the idea behind the near and far but why these values?
	glm::mat4 projection =
		glm::perspective(45.0f, (GLfloat)mainWindow.getWidth() / (GLfloat)mainWindow.getHeight(), 0.1f, 100.0f);

	// loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = (GLfloat)glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		
		// get + hanlde user input events
		mainWindow.pollEvents();

		camera.keyControll(mainWindow.getKeys(), deltaTime);
		camera.mouseControll(mainWindow.getXChange(), mainWindow.getYChange());

		// clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw stuff
		shaderList[0]->use();

		// NOTE(christian): having the uniforms all here is probably not ideal.
		GLuint uniformModel = shaderList[0]->GetModelLocation();
		GLuint uniformView = shaderList[0]->GetViewLocation();
		GLuint uniformProjection = shaderList[0]->GetProjectionLocation();
		GLuint uniformEyePosition = shaderList[0]->GetEyePositionLocation();

		GLuint uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		GLuint uniformShininess = shaderList[0]->GetShininessLocation();

		spotLights[0].setFlash((camera.getPosition() - glm::vec3(0.0f, 0.2f, 0.0f)), camera.getDirection());

		shaderList[0]->setDirectionalLight(mainLight);
		shaderList[0]->setPointLights(pointLights, pointLightCount);
		shaderList[0]->setSpotLights(spotLights, spotLightCount);

		// NOTE(christian): this looks like the camera. perhaps move this out to the camera class?
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(uniformEyePosition, 1, glm::value_ptr(camera.getPosition()));

		// draw one object
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->Render();

		// draw another object
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->Render();

		// draw floor object
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->Render();

		glUseProgram(0);

		// display the newly rendered content
		mainWindow.swapBuffers();
	}

	return 0;
}