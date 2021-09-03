#pragma once
#include <vector>
#include <memory>
#include "Window.h"
#include "Layer.h"

struct Time
{
	static float deltaTime;
};

class Application
{
public:
	Application(const char* name, unsigned int width, unsigned int height);
	virtual ~Application();

	std::shared_ptr<Window>& getWindow() { return m_Window; }
	void pushLayer(const std::shared_ptr<Layer>& layer);

	void Run();

	static Application* Get() { return m_Instance; }

private:
	std::shared_ptr<Window> m_Window;
	std::vector<std::shared_ptr<Layer>> m_Layers;

	static Application* m_Instance;

};