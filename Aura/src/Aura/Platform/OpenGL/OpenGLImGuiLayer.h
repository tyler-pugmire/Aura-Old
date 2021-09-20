#pragma once

#include "Aura/ImGui/ImGuiLayer.h"

namespace Aura
{
    class OpenGLImGuiLayer : public ImGuiLayer
    {
    public:
        OpenGLImGuiLayer(const std::string& name = "OpenGLImGuiLayer");
        virtual ~OpenGLImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void Begin() override;
        virtual void End() override;
        virtual void OnImGuiRender() override;
        virtual void OnEvent(Event& event) override;
    };
}