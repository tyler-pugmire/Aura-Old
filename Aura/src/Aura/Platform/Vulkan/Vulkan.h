#pragma once

#include <vulkan/vulkan.h>
#include "Aura/Core/Logger.h"
#include "Aura/Core/Assert.h"

namespace Aura::Utils
{
    static const char* vulkanResultToString(VkResult result) 
    {
        switch (result) 
        {
        case VK_SUCCESS: return "Success";
        case VK_NOT_READY: return "Not ready";
        case VK_TIMEOUT: return "Timeout";
        case VK_EVENT_SET: return "Event set";
        case VK_EVENT_RESET: return "Event reset";
        case VK_INCOMPLETE: return "Incomplete";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "Error: Out of host memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "Error: Out of device memory";
        case VK_ERROR_INITIALIZATION_FAILED: return "Error: Initialization failed";
        case VK_ERROR_DEVICE_LOST: return "Error: Device lost";
        case VK_ERROR_MEMORY_MAP_FAILED: return "Error: Memory map failed";
        case VK_ERROR_LAYER_NOT_PRESENT: return "Error: Layer not present";
        case VK_ERROR_EXTENSION_NOT_PRESENT: return "Error: Extension not present";
        case VK_ERROR_FEATURE_NOT_PRESENT: return "Error: Feature not present";
        case VK_ERROR_INCOMPATIBLE_DRIVER: return "Error: Incompatible driver";
        case VK_ERROR_TOO_MANY_OBJECTS: return "Error: Too many objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED: return "Error: Format not supported";
        case VK_ERROR_FRAGMENTED_POOL: return "Error: Fragmented pool";
        case VK_ERROR_SURFACE_LOST_KHR: return "Error: Surface lost";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "Error: Native window in use";
        case VK_SUBOPTIMAL_KHR: return "Suboptimal";
        case VK_ERROR_OUT_OF_DATE_KHR: return "Error: Out of date";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "Error: Incompatible display";
        case VK_ERROR_VALIDATION_FAILED_EXT: return "Error: Validation failed";
        case VK_ERROR_INVALID_SHADER_NV: return "Error: Invalid shader";
        default: return "Unknown error";
        }
    }

    inline void VulkanCheckResult(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
            AURA_CORE_ERROR("VkResult is '{0}' in {1}:{2}", ::Aura::Utils::vulkanResultToString(result), __FILE__, __LINE__);
            AURA_CORE_ASSERT(result == VK_SUCCESS);
        }
    }
}

#define VK_CHECK_RESULT(f)\
{\
	VkResult res = (f);\
	::Aura::Utils::VulkanCheckResult(res);\
}