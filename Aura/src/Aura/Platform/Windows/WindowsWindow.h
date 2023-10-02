#pragma once

#include "Aura/Core/Window.h"

struct GLFWwindow;
struct GLFWcursor;

namespace Aura
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowSpecification& spec);
        virtual ~WindowsWindow();

        virtual void Init() override;
        virtual void ProcessEvents() override;
        virtual void SwapBuffer() override;

        inline uint32_t GetWidth() const override { return m_data.width; }
        inline uint32_t GetHeight() const override { return m_data.height; }
        virtual std::pair<uint32_t, uint32_t> GetSize() const override { return { m_data.width, m_data.height }; }
        virtual std::pair<float, float> GetWindowPos() const override;

        virtual void Maximize() override;
        virtual void CenterWindow() override;

        virtual void SetEventCallback(EventCallbackFn const& fn) override;
        virtual void SetVSync(bool enabled) override;
        virtual bool IsVSync() const override;
        virtual void SetResizable(bool resizable) const override;

        virtual const std::string& GetTitle() const override { return m_data.title; }
        virtual void SetTitle(const std::string& title) override;

        virtual void* GetNativeWindow() const override { return m_window; }

    private:
        GLFWwindow* m_window;
        GLFWcursor* m_imguiMouseCursors[9] = { 0 };
        WindowSpecification m_specification;
        struct WindowData
        {
            std::string title;
            uint32_t width;
            uint32_t height;

            EventCallbackFn eventCallback;
        };

        WindowData m_data;
        float m_lastFrameTime = 0.0f;


        virtual void Shutdown();

    };
}