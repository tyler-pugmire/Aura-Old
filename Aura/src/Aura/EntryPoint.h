#pragma once

#ifdef AURA_PLATFORM_WINDOWS

#include "Aura/Core/Base.h"

extern Aura::Application* Aura::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

int main(int argc, char** argv)
{
	while (g_ApplicationRunning)
	{
		Aura::Core::Initialize();
		Aura::Application* app = Aura::CreateApplication(argc, argv);
		app->Run();
		delete app;
		Aura::Core::Shutdown();
	}
	return 0;
}

#endif