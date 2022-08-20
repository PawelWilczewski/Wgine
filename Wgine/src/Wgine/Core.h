#pragma once

#ifdef WGINE_PLATFORM_WINDOWS
	#ifdef WGINE_BUILD_DLL
		#define WGINE_API __declspec(dllexport)
	#else
		#define WGINE_API __declspec(dllimport)
	#endif
#else
	#error Unsupported platform!
#endif

#define BIT(x) (1 << x)
