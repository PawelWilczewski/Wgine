workspace "Wgine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Wgine"
	location "Wgine"
	kind "SharedLib"
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
		"%{prj.name}/thirdparty/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"WGINE_PLATFORM_WINDOWS",
			"WGINE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
