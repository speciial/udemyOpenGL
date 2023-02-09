#include "Window.h"

Window::Window(GLint windowWidth, GLint windowHeight) :
	width(windowWidth), height(windowHeight), mouseFirstMoved(true), 
	lastX(0.0f), lastY(0.0f), xChange(0.0f), yChange(0.0f)
{
	for (size_t keyIndex = 0; keyIndex < 1024; keyIndex++)
	{
		keys[keyIndex] = 0;
	}
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

	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Initialization Failed!\n");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0;
	return theChange;
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = (GLfloat) xPos;
		theWindow->lastY = (GLfloat) yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = (GLfloat) xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - (GLfloat) yPos;

	theWindow->lastX = (GLfloat) xPos;
	theWindow->lastY = (GLfloat) yPos;
}