workspace "Wgine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Includes relative to root
IncludeDir = {}
IncludeDir["GLFW"] = "Wgine/thirdparty/GLFW/include"
IncludeDir["Glad"] = "Wgine/thirdparty/Glad/include"
IncludeDir["ImGui"] = "Wgine/thirdparty/ImGui"

include "Wgine/thirdparty/GLFW"
include "Wgine/thirdparty/Glad"
include "Wgine/thirdparty/ImGui"

project "Wgine"
	location "Wgine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "WginePCH.h"
	pchsource "Wgine/src/WginePCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/thirdparty/spdlog/include",
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
		systemversion "latest"

		defines {
			"WGINE_PLATFORM_WINDOWS",
			"WGINE_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_CUSTOM"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "WGINE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "WGINE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "WGINE_DIST"
		runtime "Release"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Wgine/thirdparty/spdlog/include",
		"Wgine/src"
	}

	links
	{
		"Wgine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"WGINE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "WGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "WGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "WGINE_DIST"
		optimize "On"
