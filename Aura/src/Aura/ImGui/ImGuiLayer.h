#pragma once

#include "Aura/Core/Layer.h"

#include <string>

namespace Aura
{
    class ImGuiLayer : public Layer
    {
    public:

        void SetDarkThemeColors();
        void SetDarkThemeV2Colors();


        static ImGuiLayer* Create();

    protected:
        ImGuiLayer(const std::string& name = "ImGuiLayer");

    };
}