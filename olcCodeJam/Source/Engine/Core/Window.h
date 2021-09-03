#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

class Window
{
public:
	Window(const char* name, unsigned int width, unsigned int height);
	~Window();

	GLFWwindow* getHandle() { return m_Handle; }

	unsigned int getWidth() { return m_Width; }
	unsigned int getHeight() { return m_Height; }

	void Update();
	bool isRunning() { return m_Running; }

private:
	GLFWwindow* m_Handle;

	std::string m_Name;
	unsigned int m_Width;
	unsigned int m_Height;

	bool m_Running = true;
};