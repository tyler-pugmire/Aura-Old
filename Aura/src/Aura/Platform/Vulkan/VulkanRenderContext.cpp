#include "pch.h"
#include "VulkanRenderContext.h"
#include "GLFW/glfw3.h"
#include "Vulkan.h"

namespace
{
    bool CheckDriverAPIVersionSupport(uint32_t minimumSupportedVersion)
    {
        uint32_t instanceVersion;
        vkEnumerateInstanceVersion(&instanceVersion);

        if (instanceVersion < minimumSupportedVersion)
        {
            AURA_CORE_FATAL("Incompatible Vulkan driver version!");
            AURA_CORE_FATAL("  You have {}.{}.{}", VK_API_VERSION_MAJOR(instanceVersion), VK_API_VERSION_MINOR(instanceVersion), VK_API_VERSION_PATCH(instanceVersion));
            AURA_CORE_FATAL("  You need at least {}.{}.{}", VK_API_VERSION_MAJOR(minimumSupportedVersion), VK_API_VERSION_MINOR(minimumSupportedVersion), VK_API_VERSION_PATCH(minimumSupportedVersion));

            return false;
        }

        return true;
    }
}

namespace Aura
{
    VulkanContext::VulkanContext() : m_instance()
    {

    }
    VulkanContext::~VulkanContext()
    {

    }
    void VulkanContext::Init()
    {
        AURA_CORE_INFO_TAG("Renderer", "VulkanContext::Create");
        AURA_CORE_ASSERT(glfwVulkanSupported(), "GLFW must support Vulkan!");

        if (!CheckDriverAPIVersionSupport(VK_VERSION_1_2))
        {
            AURA_CORE_VERIFY(false);
        }

        uint32_t apiVersion;
        vkEnumerateInstanceVersion(&apiVersion);

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext = nullptr;
        appInfo.pApplicationName = "Aura";
        appInfo.applicationVersion = 1;
        appInfo.pEngineName = "AuraEngine";
        appInfo.engineVersion = 1;
        appInfo.apiVersion = apiVersion;

        VkInstanceCreateInfo instInfo = {};
        instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instInfo.pNext = nullptr;
        instInfo.flags = 0;
        instInfo.pApplicationInfo = &appInfo;

#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"
        std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        instInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
        instInfo.ppEnabledExtensionNames = instanceExtensions.data();

        VK_CHECK_RESULT(vkCreateInstance(&instInfo, nullptr, &m_instance));

        m_physicalDevice = Ref<VulkanPhysicalDevice>::Create(this);
        m_device = Ref<VulkanDevice>::Create(m_physicalDevice);

    }
    void VulkanContext::SwapBuffers()
    {
    }
}