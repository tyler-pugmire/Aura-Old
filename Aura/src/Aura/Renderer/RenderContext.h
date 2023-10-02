#pragma once

#include "Aura/Core/Base.h"

namespace Aura
{
    class RendererContext : public RefCounted
    {
    public:
        RendererContext() = default;
        virtual ~RendererContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Ref<RendererContext> Create();
    };
}