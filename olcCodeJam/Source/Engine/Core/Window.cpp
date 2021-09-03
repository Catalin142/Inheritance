#include <string>
#include <iostream>

#include "Window.h"
#include <assert.h>

void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei lenght,
	const GLchar* message, const void* data)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:   std::cout << message << std::endl; assert(0);  return;
	case GL_DEBUG_SEVERITY_MEDIUM: std::cout << message << std::endl; assert(0);  return;
	default:																	  return;
	}
}

Window::Window(const char* name, unsigned int width, unsigned int height) : m_Name(name), m_Width(width), m_Height(height)
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, false);

	m_Handle = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_Handle);

	if (glewInit() != GLEW_OK)
		std::cout << "glew" << std::endl;

#if DEBUG == 1
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(GLDebugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_TRUE);
#endif

	glfwSwapInterval(1);
}

Window::~Window()
{
	glfwDestroyWindow(m_Handle);
	glfwTerminate();
}

void Window::Update()
{
	glfwSwapBuffers(m_Handle);
	glfwPollEvents();

	m_Running = !glfwWindowShouldClose(m_Handle);
}
