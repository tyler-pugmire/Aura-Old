#include "Application.h"
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include <filesystem>

extern bool g_ApplicationRunning;
namespace Aura
{

	Application::Application(const ApplicationSpecification& specification)
		: appSpec(specification)
	{
		if (!specification.WorkingDirectory.empty())
			std::filesystem::current_path(specification.WorkingDirectory);

		window = (Window::Create(specification.Name, specification.WindowWidth, specification.WindowHeight));
		window->SetEventCallback([this](Event& e) { OnEvent(e); });
		window->Init();
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& event)
	{
		Aura::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Aura::WindowClosedEvent>([this](Aura::WindowClosedEvent const& e) { return onWindowClosedEvent(e); });
	}

	void Application::Run()
	{
		OnInit();
		while (running)
		{
			window->ProcessEvents();
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
}