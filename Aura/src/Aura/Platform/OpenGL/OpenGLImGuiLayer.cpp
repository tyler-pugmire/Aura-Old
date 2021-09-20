#include "OpenGLImGuiLayer.h"

#include "Aura/Core/Events/MouseEvent.h"
#include "Aura/Core/Events/KeyEvent.h"
#include "Aura/Core/Events/WindowEvent.h"


#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API
#endif
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
#include "SDL/SDL.h"

namespace Aura
{
    OpenGLImGuiLayer::OpenGLImGuiLayer(const std::string& name) 
        : ImGuiLayer(name)
    {

    }

    OpenGLImGuiLayer::~OpenGLImGuiLayer()
    {

    }

    void OpenGLImGuiLayer::OnAttach()
    {
        SDL_Window* window = SDL_GL_GetCurrentWindow();
        SDL_GLContext context = SDL_GL_GetCurrentContext();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 410");
        ImGui::StyleColorsDark();
        SetDarkThemeColors();
    }

    void OpenGLImGuiLayer::OnDetach()
    {

    }

    void OpenGLImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLImGuiLayer::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* window = SDL_GL_GetCurrentWindow();
            SDL_GLContext context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(window, context);
        }
    }

    void OpenGLImGuiLayer::OnImGuiRender()
    {

    }

    void OpenGLImGuiLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatch(event);
        ImGuiIO& io = ImGui::GetIO();
        dispatch.Dispatch<MouseScroll>([&io](const MouseScroll& scroll) {
            if (scroll.horizontal > 0) io.MouseWheelH += 1;
            if (scroll.horizontal < 0) io.MouseWheelH -= 1;
            if (scroll.vertical > 0) io.MouseWheel += 1;
            if (scroll.vertical < 0) io.MouseWheel -= 1;
            return true;
            });
        
        dispatch.Dispatch<TextInput>([&io](const TextInput& text) {
            io.AddInputCharactersUTF8(text.text);
            return true;
            });

        dispatch.Dispatch<WindowClosedEvent>([&io](const WindowClosedEvent& windowEvent) {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle((void*)SDL_GetWindowFromID(windowEvent.windowId)))
            {
                viewport->PlatformRequestClose = true;
            }
            return false;
            });
    }
}