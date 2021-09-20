#include "Window.h"

#include "SDL/SDL.h"
#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Aura/Renderer/RenderContext.h"

#include <iostream>

namespace
{
    struct WindowImpl
    {
        SDL_Window* window = nullptr;
        std::unordered_map<uint16_t, std::function<void(Aura::Window::EventCallbackFn const& fn, SDL_Event*)>> eventHandlers;
        void _handleOnExit(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            Aura::WindowClosedEvent windowClosedEvent;
            windowClosedEvent.windowId = event->window.windowID;
            fn(windowClosedEvent);
        }

        void _handleWindowEvent(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            switch (event->window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                _handleOnExit(fn, event);
                break;
            default:
                break;
            }
        }

        void _handleMouseScroll(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            Aura::MouseScroll mouseScroll;
            mouseScroll.horizontal = event->wheel.x;
            mouseScroll.vertical = event->wheel.y;
            fn(mouseScroll);
        }

        void _handleMouseButtonDown(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            Aura::MouseButtonDown mouse;
            mouse.button = static_cast<Aura::MouseButton>(event->button.button);
            fn(mouse);
        }

        void _handleTextInput(Aura::Window::EventCallbackFn const& fn, SDL_Event* event)
        {
            Aura::TextInput textInput;
            memcpy(textInput.text, event->text.text, 32);
            fn(textInput);
        }
    };
}

namespace Aura
{
    Window* Window::Create(std::string const& title, uint32_t width, uint32_t height)
    {
        Window* window = new Window();

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            printf("Couldn't initialize SDL: %s\n", SDL_GetError());
            delete window;
            return nullptr;
        }

        window->window->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);

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
        renderContext = RenderContext::Create(window->window);
        renderContext->Init();


#define BIND_EVENT(eventFn) std::bind(&##eventFn, window, std::placeholders::_1, std::placeholders::_2)
        window->eventHandlers[SDL_QUIT] = BIND_EVENT(WindowImpl::_handleOnExit);
        window->eventHandlers[SDL_WINDOWEVENT] = BIND_EVENT(WindowImpl::_handleWindowEvent);
        window->eventHandlers[SDL_MOUSEWHEEL] = BIND_EVENT(WindowImpl::_handleMouseScroll);
        window->eventHandlers[SDL_MOUSEBUTTONDOWN] = BIND_EVENT(WindowImpl::_handleMouseButtonDown);
        window->eventHandlers[SDL_TEXTINPUT] = BIND_EVENT(WindowImpl::_handleTextInput);
    }

    void Window::ProcessEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            auto& fn = window->eventHandlers.find(event.type);
            if (fn != window->eventHandlers.end())
                fn->second(eventCallback, &event);
        }
    }

    void Window::SwapBuffer()
    {
        renderContext->SwapBuffers();
    }

    void* Window::GetNativeWindow()
    {
        return window->window;
    }

    void Window::Maximize()
    {
        SDL_MaximizeWindow(window->window);
    }
}