#pragma once

#include "Aura/Renderer/RenderContext.h"
#include "SDL/SDL.h"

namespace
{
    struct ContextImpl;
}

namespace Aura
{
    class OpenGLContext : public RenderContext
    {
    public:
        OpenGLContext(SDL_Window* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        ContextImpl* impl;
    };
}