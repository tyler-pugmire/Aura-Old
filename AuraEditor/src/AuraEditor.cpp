#include "Aura.h"
#include "Aura/EntryPoint.h"
#include "imgui.h"
#include "EditorLayer.h"
#include <iostream>

class AuraEditorApplication : public Aura::Application
{
public:
	AuraEditorApplication(const Aura::ApplicationSpecification& specification) : Aura::Application(specification)
	{

	}

	void OnInit() override
	{
		const Aura::ApplicationSpecification& spec = GetSpecification();
		std::cout << "Application Specification: " << std::endl;
		std::cout << "\tName: " << spec.Name << std::endl;
		std::cout << "\tWindowWidth: " << spec.WindowWidth << std::endl;
		std::cout << "\tWindowHeight: " << spec.WindowHeight << std::endl;
		std::cout << "\tvSync: " << spec.VSync << std::endl;
		std::cout << "\tStartMaximized: " << spec.StartMaximized << std::endl;
		std::cout << "\tResizable: " << spec.Resizable << std::endl;
		std::cout << "\tWorkingDirectory: " << spec.WorkingDirectory << std::endl;
		std::cout << "\tWindowMode: ";
		switch (spec.WindowMode)
		{
		case Aura::WindowMode::Windowed:
			std::cout << "Windowed" << std::endl;
			break;
		case Aura::WindowMode::Fullscreen:
			std::cout << "Fullscreen" << std::endl;
			break;
		case Aura::WindowMode::BorderlessWindowed:
			std::cout << "BorderlessWindowed" << std::endl;
			break;
		}
		sandboxLayer = new EditorLayer;
		PushLayer(sandboxLayer);
	}

	void OnShutdown() override
	{
		Aura::Application::OnShutdown();
	}
	EditorLayer* sandboxLayer = nullptr;
};

Aura::Application* Aura::CreateApplication(int argc, char** argv)
{
	Aura::ApplicationSpecification specification;
	specification.Name = "Sandbox";
	specification.WindowWidth = 1280;
	specification.WindowHeight = 720;
	specification.VSync = true;
	specification.StartMaximized = false;
	specification.Resizable = false;
	specification.WorkingDirectory = "../Sandbox";

	return new AuraEditorApplication(specification);
}