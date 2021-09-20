#include "SandboxLayer.h"
#include "imgui.h"

static bool show = true;
SandboxLayer::SandboxLayer() : Layer("Sandbox Layer")
{

}

void SandboxLayer::OnImGuiRender()
{
    if(show)
        ImGui::ShowDemoWindow(&show);
}