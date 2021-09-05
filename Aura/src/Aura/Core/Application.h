#pragma once

namespace Aura
{
    class Application
    {
    public:
        void Run();
    };

    Application* CreateApplication(int argc, char** argv);
}