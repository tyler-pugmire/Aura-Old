#pragma once

#include <memory>
struct SDL_Window;
namespace Aura
{
    class RenderContext
    {
    public:
        RenderContext() = default;
        virtual ~RenderContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static RenderContext* Create(SDL_Window* window);
    };
}