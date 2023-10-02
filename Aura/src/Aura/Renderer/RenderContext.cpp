#include "pch.h"
#include "RenderContext.h"

#include "Aura/Platform/Vulkan/VulkanRenderContext.h"

namespace Aura
{
    Ref<RendererContext> RendererContext::Create()
    {
        return Ref<VulkanContext>::Create();
    }
}