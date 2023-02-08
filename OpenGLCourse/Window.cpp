#include "Window.h"

Window::Window(GLint windowWidth, GLint windowHeight) :
	width(windowWidth), height(windowHeight)
{

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

int Window::initialize()
{
	// initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization Failed!\n");
		glfwTerminate();
		return 1;
	}

	// setup GLFW window properties
	// openGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// NOTE(christian): What do the last parameters do here?
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW Window Creation Failed!\n");
		glfwTerminate();
		return 1;
	}

	// get buffer size information
	// NOTE(christian): do the farme buffer width and height differ from the width and height 
	//					parameter passed in the glfwCreateWindow() call?
	//					ANSWER -> based on limited testing, the widht and height values are 
	//					equal to bufferWidth and bufferHeight. according to some comments there
	//					cases where this does not work.
	// int bufferWidth;
	// int bufferHeight;
	// glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Initialization Failed!\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	return 0;
}