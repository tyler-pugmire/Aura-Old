#include "pch.h"
#include "VulkanSwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace Aura
{
    /*
    void VulkanSwapChain::Init(Ref<RendererContext> rendererContext)
    {
        m_rendererContext = static_cast<Ref<VulkanContext>>(rendererContext);
    }

    void VulkanSwapChain::InitSurface(GLFWwindow* windowHandle)
    {
        m_instance = m_rendererContext->GetInstance();
        VkPhysicalDevice physicalDevice = m_rendererContext->GetPhysicalDevice()->GetVulkanPhysicalDevice();
        glfwCreateWindowSurface(m_instance, windowHandle, nullptr, &m_surface);

        uint32_t queueCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
        AURA_CORE_ASSERT(queueCount >= 1);

        std::vector<VkQueueFamilyProperties> queueProps(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

        std::vector<VkBool32> supportsPresent(queueCount);
        for (uint32_t i = 0; i < queueCount; i++)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_surface, &supportsPresent[i]);
        }

        uint32_t graphicsQueueNodeIndex = UINT32_MAX;
        uint32_t presentQueueNodeIndex = UINT32_MAX;
        for (uint32_t i = 0; i < queueCount; i++)
        {
            if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            {
                if (graphicsQueueNodeIndex == UINT32_MAX)
                {
                    graphicsQueueNodeIndex = i;
                }

                if (supportsPresent[i] == VK_TRUE)
                {
                    graphicsQueueNodeIndex = i;
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }
        if (presentQueueNodeIndex == UINT32_MAX)
        {
            // If there's no queue that supports both present and graphics
            // try to find a separate present queue
            for (uint32_t i = 0; i < queueCount; ++i)
            {
                if (supportsPresent[i] == VK_TRUE)
                {
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }

        AURA_CORE_ASSERT(graphicsQueueNodeIndex != UINT32_MAX);
        AURA_CORE_ASSERT(presentQueueNodeIndex != UINT32_MAX);

        m_queueNodeIndex = graphicsQueueNodeIndex;
        FindImageFormatAndColorSpace();
    }

    void VulkanSwapChain::Create(uint32_t* width, uint32_t* height, bool vsync)
    {
        VkPhysicalDevice physicalDevice = m_rendererContext->GetPhysicalDevice()->GetVulkanPhysicalDevice();

        VkSwapchainKHR oldSwapchain = m_swapchain;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &surfaceCapabilities));

        uint32_t presentModeCount;
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, NULL));
        AURA_CORE_ASSERT(presentModeCount > 0);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, presentModes.data()));

        VkExtent2D swapchainExtent = {};
        // If width (and height) equals the special value 0xFFFFFFFF, the size of the surface will be set by the swapchain
        if (surfaceCapabilities.currentExtent.width == (uint32_t)-1)
        {
            // If the surface size is undefined, the size is set to
            // the size of the images requested.
            swapchainExtent.width = *width;
            swapchainExtent.height = *height;
        }
        else
        {
            // If the surface size is defined, the swap chain size must match
            swapchainExtent = surfaceCapabilities.currentExtent;
            *width = surfaceCapabilities.currentExtent.width;
            *height = surfaceCapabilities.currentExtent.height;
        }

        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        // If v-sync is not requested, try to find a mailbox mode
        // It's the lowest latency non-tearing present mode available
        if (!vsync)
        {
            for (size_t i = 0; i < presentModeCount; i++)
            {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
                {
                    swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
                    break;
                }
                if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
                {
                    swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
                }
            }
        }

        unsigned int swapChainCount = surfaceCapabilities.minImageCount + 1;
        swapChainCount = swapChainCount > surfaceCapabilities.maxImageCount ? surfaceCapabilities.maxImageCount : swapChainCount;

        VkSurfaceTransformFlagsKHR preTransform;
        if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        {
            // We prefer a non-rotated transform
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else
        {
            preTransform = surfaceCapabilities.currentTransform;
        }

        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        // Simply select the first composite alpha format available
        std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = 
        {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        };
        for (auto& compositeAlphaFlag : compositeAlphaFlags) 
        {
            if (surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlag) 
            {
                compositeAlpha = compositeAlphaFlag;
                break;
            };
        }

        VkSwapchainCreateInfoKHR swapInfo;
        swapInfo.pNext = nullptr;
        swapInfo.flags = 0;
        swapInfo.surface = m_surface;
        swapInfo.minImageCount = swapChainCount;
        swapInfo.imageFormat = m_colorFormat;
        swapInfo.imageColorSpace = m_colorSpace;
        swapInfo.imageExtent = swapchainExtent;
        swapInfo.imageArrayLayers = 1;
        swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapInfo.queueFamilyIndexCount = 0;
        swapInfo.pQueueFamilyIndices = nullptr;
        swapInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
        swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapInfo.presentMode = swapchainPresentMode;
        swapInfo.clipped = true;
        swapInfo.oldSwapchain = oldSwapchain;
        swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

        // Enable transfer source on swap chain images if supported
        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
            swapInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        // Enable transfer destination on swap chain images if supported
        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
            swapInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }

        VkDevice device = m_rendererContext->GetDevice()->GetVulkanDevice();
        VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &swapInfo, nullptr, &m_swapchain));

        if (oldSwapchain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
            oldSwapchain = VK_NULL_HANDLE;
        }


    }

    void VulkanSwapChain::Destroy()
    {

    }

    void VulkanSwapChain::OnResize(uint32_t width, uint32_t height)
    {

    }

    void VulkanSwapChain::BeginFrame()
    {

    }

    void VulkanSwapChain::Present()
    {

    }

    void VulkanSwapChain::FindImageFormatAndColorSpace()
    {
        VkPhysicalDevice physicalDevice = m_rendererContext->GetPhysicalDevice()->GetVulkanPhysicalDevice();

        unsigned int count(0);
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &count, nullptr));
        AURA_CORE_ASSERT(count > 0);


        std::vector<VkSurfaceFormatKHR> foundFormats(count);
        VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &count, foundFormats.data()));

        if ((count == 1) && (foundFormats[0].format == VK_FORMAT_UNDEFINED))
        {
            m_colorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            m_colorSpace = foundFormats[0].colorSpace;
        }
        else
        {
            bool found_B8G8R8A8_UNORM = false;
            for (auto&& surfaceFormat : foundFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    m_colorFormat = surfaceFormat.format;
                    m_colorSpace = surfaceFormat.colorSpace;
                    found_B8G8R8A8_UNORM = true;
                    break;
                }
            }

            if (!found_B8G8R8A8_UNORM)
            {
                m_colorFormat = foundFormats[0].format;
                m_colorSpace = foundFormats[0].colorSpace;
            }
        }
    }
    */
}