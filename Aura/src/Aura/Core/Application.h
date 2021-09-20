#pragma once
#include "Window.h"
#include "LayerStack.h"

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

	class ImGuiLayer;
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
		void RenderImGui();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		inline const ApplicationSpecification& GetSpecification() const { return appSpec; }
		inline Window* GetWindow() { return window; }

		static inline Application& Get() { return *Instance; }

	private:
		ApplicationSpecification appSpec;
		bool running = true;
		Window* window;
		LayerStack layerStack;
		ImGuiLayer* imGuiLayer;

		bool onWindowClosedEvent(Aura::WindowClosedEvent const& e);

		static Application* Instance;
    };

    Application* CreateApplication(int argc, char** argv);
}