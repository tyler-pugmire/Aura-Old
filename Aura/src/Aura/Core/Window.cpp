#include "pch.h"
#include "Window.h"

#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Aura/Renderer/RenderContext.h"

#ifdef AURA_PLATFORM_WINDOWS
#include "Aura/Platform/Windows/WindowsWindow.h"
#endif

#include <iostream>

namespace Aura
{
    Window* Window::Create(const WindowSpecification& specification)
    {
        //Window* window = new Window();
#ifdef AURA_PLATFORM_WINDOWS
        return new WindowsWindow(specification);
#else

#endif

        return nullptr;
    }
}
