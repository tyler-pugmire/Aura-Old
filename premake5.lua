workspace "Aura"
	architecture "x64"
	targetdir "build"
	
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

	startproject "Sandbox"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

group "Engine"
project "Aura"
    location "Aura"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.c", 
		"%{prj.name}/src/**.hpp", 
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
        "%{IncludeDir.SDL}"
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

group ""
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links 
	{ 
		"Aura",
        "%{Library.SDL}"
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
		"Aura/src"
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

