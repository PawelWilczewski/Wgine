#pragma once

#ifdef WGINE_PLATFORM_WINDOWS
#if WGINE_DYNAMIC_LINK
	#ifdef WGINE_BUILD_DLL
		#define WGINE_API __declspec(dllexport)
	#else
		#define WGINE_API __declspec(dllimport)
	#endif
#else
	#define WGINE_API
#endif
#else
	#error Unsupported platform!
#endif

#ifdef WGINE_DEBUG
	#define WGINE_ENABLE_ASSERTS
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

// TODO: ifdef using opengl
// using openglleft handed system
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
