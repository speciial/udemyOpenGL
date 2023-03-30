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

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

// window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// shader file paths
const char* vertexShaderPath = "shaders/shader.vert";
const char* fragmentShaderPath = "shaders/shader.frag";

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
		// x      y     z      u     v
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 0.0f, -1.0f, 1.0f,  0.5f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,  0.5f, 1.0f
	};

	// NOTE(christian): I still don't understand why there are "create" calls instead of just
	//					using the contructor. 
	Mesh* mesh1 = new Mesh();
	mesh1->Create(vertices, indices, 20, 12);
	meshList.push_back(mesh1);

	Mesh* mesh2 = new Mesh();
	mesh2->Create(vertices, indices, 20, 12);
	meshList.push_back(mesh2);
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
		GLuint uniformModel = shaderList[0]->GetModelLocation();
		GLuint uniformView = shaderList[0]->GetViewLocation();
		GLuint uniformProjection = shaderList[0]->GetProjectionLocation();

		// draw one object
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		brickTexture.UseTexture();
		meshList[0]->Render();

		// draw another object
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, -5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		dirtTexture.UseTexture();
		meshList[1]->Render();

		glUseProgram(0);

		// display the newly rendered content
		mainWindow.swapBuffers();
	}

	return 0;
}