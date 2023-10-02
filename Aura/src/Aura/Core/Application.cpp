#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include <filesystem>
#include "Aura/ImGui/ImGuiLayer.h"

#include "glad/glad.h"

extern bool g_ApplicationRunning;
namespace Aura
{
	Application* Application::Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: appSpec(specification)
	{
		Instance = this;
		if (!specification.WorkingDirectory.empty())
			std::filesystem::current_path(specification.WorkingDirectory);

		WindowSpecification windowSpec;
		windowSpec.title = specification.Name;
		windowSpec.width = specification.WindowWidth;
		windowSpec.height = specification.WindowHeight;


		window = Window::Create(windowSpec);
		window->SetEventCallback([this](Event& e) { OnEvent(e); });
		window->Init();
		
		//imGuiLayer = ImGuiLayer::Create();
		//layerStack.PushOverlay(imGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& event)
	{
		Aura::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Aura::WindowClosedEvent>([this](WindowClosedEvent const& e) { return onWindowClosedEvent(e); });

		for (Layer* layer : layerStack)
		{
			if (event.handled)
				break;
			layer->OnEvent(event);
		}
	}

	void Application::Run()
	{
		OnInit();
		while (running)
		{
			window->ProcessEvents();

			for (Layer* layer : layerStack)
				layer->OnUpdate(1.0f / 60.0f);

			//glClearColor(.3f, .3f, .3f, 1);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//RenderImGui();

			window->SwapBuffer();

		}
		OnShutdown();
	}

	void Application::Close()
	{
		running = false;
	}

	void Application::OnShutdown()
	{
		g_ApplicationRunning = false;
	}

	bool Application::onWindowClosedEvent(Aura::WindowClosedEvent const& e)
	{
		Close();
		return true;
	}

	void Application::RenderImGui()
	{
		imGuiLayer->Begin();
		for (Layer* layer : layerStack)
			layer->OnImGuiRender();
		imGuiLayer->End();
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushOverlay(overlay);
	}

	void Application::PopLayer(Layer* layer)
	{
		layerStack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		layerStack.PopOverlay(overlay);
	}
}
