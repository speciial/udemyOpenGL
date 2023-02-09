#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(GLint windowWidth, GLint windowHeight);
	~Window();

	int initialize();

	GLint getWidth() { return width; }
	GLint getHeight() { return height; }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void pollEvents() { glfwPollEvents(); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLint width;
	GLint height;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void createCallbacks();

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

