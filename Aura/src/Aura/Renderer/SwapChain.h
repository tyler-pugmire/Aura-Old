#pragma once
#include "Aura/Core/Base.h"
#include "Aura/Renderer/RenderContext.h"

struct GLFWwindow;

namespace Aura
{
    class SwapChain : public RefCounted
    {
    public:
        virtual void Init(Ref<RendererContext> rendererContext) = 0;
        virtual void InitSurface(GLFWwindow* windowHandle) = 0;
        virtual void Create(uint32_t* width, uint32_t* height, bool vsync) = 0;
        virtual void Destroy() = 0;

        virtual void OnResize(uint32_t width, uint32_t height) = 0;

        virtual void BeginFrame() = 0;
        virtual void Present() = 0;
    private:
    };
}