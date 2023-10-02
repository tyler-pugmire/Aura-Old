#pragma once

#include "Aura/Renderer/RenderContext.h"
#include <vulkan/vulkan.h>
#include "Aura/Platform/Vulkan/VulkanDevice.h"

namespace Aura
{
    class VulkanContext : public RendererContext
    {
    public:
        VulkanContext();
        virtual ~VulkanContext();

        virtual void Init() override;
        virtual void SwapBuffers() override;

        inline VkInstance GetInstance() const { return m_instance; }

        Ref<VulkanDevice> GetDevice() const { return m_device; }
        Ref<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_physicalDevice; }

    private:
        VkInstance m_instance;
        Ref<VulkanPhysicalDevice> m_physicalDevice;
        Ref<VulkanDevice> m_device;
    };
}