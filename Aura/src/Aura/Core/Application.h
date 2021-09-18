#pragma once
#include "Window.h"

#include <string>
#include <memory>

namespace Aura
{
	struct ApplicationSpecification
	{
		std::string Name = "Aura";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		WindowMode WindowMode = WindowMode::BorderlessWindowed;
		bool VSync = true;
		std::string WorkingDirectory;
		bool StartMaximized = true;
		bool Resizable = true;
		bool EnableImGui = true;
	};

	class Window;
	class WindowClosedEvent;
    class Application
    {
    public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void Close();
		void OnEvent(Event& event);

		virtual void OnInit() {}
		virtual void OnShutdown();

		inline const ApplicationSpecification& GetSpecification() const { return appSpec; }

	private:
		ApplicationSpecification appSpec;
		bool running = true;
		Window* window;

		bool onWindowClosedEvent(Aura::WindowClosedEvent const& e);
    };

    Application* CreateApplication(int argc, char** argv);
}