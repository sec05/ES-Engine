workspace "Engine"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Engine/vendor/imgui"

include "Engine/vendor/GLFW"
include "Engine/vendor/Glad"
include "Engine/vendor/imgui"
project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	pchheader "espch.h"
	pchsource "Engine/src/espch.cpp"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"ES_PLATFORM_WINDOWS",
			"ES_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	filter "configurations:Debug"
		defines "ES_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	filter "configurations:Release"
		defines "ES_RELEASE"
		buildoptions "/MD"
		optimize "On"
	filter "configurations:Dist"
		defines "ES_DIST"
		buildoptions "/MD"
		optimize "On"
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src"
	}
	links
	{
		"Engine"
	}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"ES_PLATFORM_WINDOWS"
		}
	filter "configurations:Debug"
		defines "ES_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	filter "configurations:Release"
		defines "ES_RELEASE"
		buildoptions "/MD"
		optimize "On"
	filter "configurations:Dist"
		defines "ES_DIST"
		buildoptions "/MD"
		optimize "On"