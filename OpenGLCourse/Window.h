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

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	void pollEvents() { glfwPollEvents(); }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

private:
	GLFWwindow* mainWindow;

	GLint width;
	GLint height;
};

