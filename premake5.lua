include "Dependencies.lua"

workspace "Aura"
	architecture "x64"
	targetdir "build"
	startproject "AuraEditor"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
include "Aura/Dependencies/GLFW"
include "Aura/Dependencies/ImGui"
group ""

group "Engine"
project "Aura"
    location "Aura"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Aura/src/pch.cpp"

    files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/Dependencies/VulkanMemoryAllocator/**.h",
		"%{prj.name}/Dependencies/VulkanMemoryAllocator/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.mustache}",
		"%{IncludeDir.GLFW}",
	}

	links
	{
		"ImGui",
		"GLFW",
		"%{Library.Vulkan}",
		"%{Library.VulkanUtils}",
	}

	filter "system:windows"
		systemversion "latest"
		
		defines 
		{ 
			"AURA_PLATFORM_WINDOWS",
			"AURA_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "AURA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines
		{
			"AURA_RELEASE",
			"NDEBUG" -- PhysX Requires This
		}

		optimize "On"

	filter "configurations:Dist"
		defines "AURA_DIST"
		optimize "On"

group "Tools"
project "AuraEditor"
	location "AuraEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"Aura",
	}

	defines 
	{
		"GLM_FORCE_DEPTH_ZERO_TO_ONE"
	}
	
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"Aura/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.SDL}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Vulkan}",
	}

    --[[postbuildcommands 
    {
        '{COPY} "%{LibraryDir.SDL}/SDL2.dll" "%{cfg.targetdir}"'
    }]]--

	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"AURA_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "AURA_DEBUG"
		symbols "on"

				
	filter "configurations:Release"
		defines
		{
			"AURA_RELEASE",
			"NDEBUG" -- PhysX Requires This
		}

		optimize "on"

	filter "configurations:Dist"
		defines "AURA_DIST"
		optimize "on"
group ""

--[[
group ""
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"Aura",
        "%{Library.SDL}",
		"ImGui"
	}
	
	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"Aura/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.spdlog}"
	}

    postbuildcommands 
    {
        '{COPY} "%{LibraryDir.SDL}/SDL2.dll" "%{cfg.targetdir}"'
    }

	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"AURA_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "AURA_DEBUG"
		symbols "on"

				
	filter "configurations:Release"
		defines
		{
			"AURA_RELEASE",
			"NDEBUG" -- PhysX Requires This
		}

		optimize "on"

	filter "configurations:Dist"
		defines "AURA_DIST"
		optimize "on"

]]--