#pragma once

#ifdef AURA_PLATFORM_WINDOWS

extern Aura::Application* Aura::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

int main(int argc, char** argv)
{
	while (g_ApplicationRunning)
	{
		Aura::Application* app = Aura::CreateApplication(argc, argv);
		app->Run();
		delete app;
	}
}

#endif