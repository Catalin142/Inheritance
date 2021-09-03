#include <memory>
#include <iostream>

#include "Application.h"
#include "Engine/Graphics/Renderer.h"

float Time::deltaTime = 0;

Application* Application::m_Instance;

Application::Application(const char* name, unsigned int width, unsigned int height)
{
	if (!glfwInit())
		std::cout << "glfw" << std::endl;
	m_Window = std::make_shared<Window>(name, width, height);
	Renderer::Init();

	m_Instance = this;
}

Application::~Application()
{
	m_Layers.clear();
}

void Application::pushLayer(const std::shared_ptr<Layer>& layer)
{
	m_Layers.push_back(layer);
	layer->onAttach();
}

void Application::Run()
{
	float currentTime = (float)glfwGetTime();
	float lastTime = currentTime;
	while (m_Window->isRunning())
	{
		currentTime = (float)glfwGetTime();
		Time::deltaTime = currentTime - lastTime;

		for (const auto& layer : m_Layers)
			if (layer->m_isActive)
				layer->onUpdate(Time::deltaTime);

		m_Window->Update();

		lastTime = currentTime;
	}
}
