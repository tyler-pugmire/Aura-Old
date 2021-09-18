#include "Window.h"

#include "SDL/SDL.h"
#include "Events/WindowEvent.h"

#include <iostream>

namespace
{
    struct WindowImpl
    {
        SDL_Window* window;
        std::unordered_map<uint16_t, std::function<void(Aura::Window::EventCallbackFn const& fn, SDL_Event*)>> eventHandlers;
        void _handleOnExit(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            Aura::WindowClosedEvent windowClosedEvent;
            fn(windowClosedEvent);
        }
    };
}

namespace Aura
{
    Window* Window::Create(std::string const& title, uint32_t width, uint32_t height)
    {
        Window* window = new Window();
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            delete window;
            return nullptr;
        }

        window->window->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

        if (!window->window->window)
        {
            printf("Failed to open %u x %u window: %s\n", width, height, SDL_GetError());
            delete window;
            return nullptr;
        }

        return window;
    }


    Window::Window() : window(new WindowImpl())
    {

    }

    void Window::Init()
    {
#define BIND_EVENT(eventFn) std::bind(&##eventFn, window, std::placeholders::_1, std::placeholders::_2)
        window->eventHandlers[SDL_QUIT] = BIND_EVENT(WindowImpl::_handleOnExit);
    }

    void Window::ProcessEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            auto& fn = window->eventHandlers.find(event.type);
            if (fn != window->eventHandlers.end())
                fn->second(eventCallback, &event);
            else
                std::cout << "Unhandled event " << event.type << std::endl;
        }
    }


}