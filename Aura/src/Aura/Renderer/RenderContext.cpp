#include "RenderContext.h"

#include "Aura/Platform/OpenGL/OpenGLContext.h"

namespace Aura
{
    RenderContext* RenderContext::Create(SDL_Window* window)
    {
        return new OpenGLContext(window);
    }
}