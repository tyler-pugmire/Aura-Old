#include "Aura.h"
#include "Aura/EntryPoint.h"



class SandApplication : public Aura::Application
{

};

Aura::Application* Aura::CreateApplication(int argc, char** argv)
{
	return new SandApplication();
}