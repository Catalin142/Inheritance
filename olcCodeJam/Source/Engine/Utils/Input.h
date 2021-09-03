#pragma once

#include "Engine/Core/Application.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

static bool isPressed(unsigned int keyCode)
{
	if (glfwGetKey(Application::Get()->getWindow()->getHandle(), keyCode) == GLFW_PRESS)
		return true;
	return false;
}

static glm::vec2 getMousePosition()
{
	double x, y;
	glfwGetCursorPos(Application::Get()->getWindow()->getHandle(), &x, &y);

	return glm::vec2((float)x, (float)y);
}

static glm::vec2 getMouseInverted()
{
	auto Window = Application::Get()->getWindow()->getHandle();
	double xpos, ypos;
	glfwGetCursorPos(Window, &xpos, &ypos);
	int width, height;
	glfwGetWindowSize(Window, &width, &height);
	return glm::vec2((float)xpos, -(float)ypos + height);
}

static glm::vec2 getMouseWorld(const std::shared_ptr<Camera> cam)
{
	glm::vec3 mousePosition = glm::translate(glm::mat4(1.0f), cam->getPosition()) * glm::vec4(getMouseInverted(), 1.0f, 1.0f);

	return { mousePosition.x, mousePosition.y };
}

static bool isMousePressed(unsigned int keyCode)
{
	if (glfwGetMouseButton(Application::Get()->getWindow()->getHandle(), keyCode) == GLFW_PRESS)
		return true;
	return false;
}