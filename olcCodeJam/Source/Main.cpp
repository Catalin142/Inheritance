#include <iostream>

#include "Engine/Core/Application.h"
#include "Game/MainLayer.h"

class olcCodeJam : public Application
{
public:
	olcCodeJam(const char* name, unsigned int width, unsigned int height) : Application(name, width, height) 
	{ 
		pushLayer(std::make_shared<MainLayer>());
	}
};


int main()
{
	Application* app = new olcCodeJam("Inheritance", 1600, 900);
	app->Run();

	delete app;
	return 0;
}