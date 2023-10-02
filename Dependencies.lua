VULKAN_SDK = os.getenv("VULKAN_SDK")
VENDOR_DIR = "%{wks.location}/Aura/Dependencies"

IncludeDir = {}
IncludeDir["stb"] = "%{VENDOR_DIR}/stb/include"
IncludeDir["GLFW"] = "%{VENDOR_DIR}/GLFW/include"
IncludeDir['ImGui'] = "%{VENDOR_DIR}/imgui"
IncludeDir['Glad'] = "%{VENDOR_DIR}/Glad/include"
IncludeDir['spdlog'] = "%{VENDOR_DIR}/spdlog/include"
IncludeDir["entt"] = "%{VENDOR_DIR}/entt/include"
IncludeDir["glm"] = "%{VENDOR_DIR}/glm"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["mustache"] = "%{VENDOR_DIR}/mustache/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"
