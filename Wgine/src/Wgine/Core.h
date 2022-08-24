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

#ifdef WGINE_ENABLE_ASSERTS
	#define WGINE_ASSERT(x, ...) { if(!(x)) { WGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define WGINE_CORE_ASSERT(x, ...) { if(!(x)) { WGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define WGINE_ASSERT(x, ...)
	#define WGINE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define WGINE_BIND_EVENT_FN(fn) (std::bind(&fn, this, std::placeholders::_1))
