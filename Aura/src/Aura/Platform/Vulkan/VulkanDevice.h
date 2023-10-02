#pragma once
#include "Aura/Renderer/Device.h"
#include "Vulkan.h"


namespace Aura
{
    class VulkanPhysicalDevice : public PhysicalDevice
    {
    public:
        struct QueueFamilyIndices
        {
            int32_t Graphics = -1;
            int32_t Compute = -1;
            int32_t Transfer = -1;
        };

        VulkanPhysicalDevice(Ref<RendererContext> rendererContext);
        ~VulkanPhysicalDevice();
        //inline VkPhysicalDevice GetVulkanPhysicalDevice() const { return m_physicalDevice; }

        bool IsExtensionSupported(const std::string& extenstion) const;
    private:


        friend class VulkanDevice;
    };

    class VulkanDevice : public RendererDevice
    {
    public:
        VulkanDevice(Ref<VulkanPhysicalDevice> physicalDevice);
        virtual ~VulkanDevice();

        inline VkDevice GetVulkanDevice() const { return m_device; }

    private:
        VkDevice m_device;
        Ref<VulkanPhysicalDevice> m_physicalDevice;
    };
}