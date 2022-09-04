#pragma once

// PLATFORMS
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

// RENDERING APIS
// TODO: ifdef using opengl
// using openglleft handed system
#define GLM_FORCE_LEFT_HANDED
// using clip space from 0 to 1 (like in directx?) instead of -1 to 1
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE

// CONFIGS
#ifdef WGINE_DEBUG
	#define WGINE_ENABLE_ASSERTS
#endif

// ASSERTS
#ifdef WGINE_ENABLE_ASSERTS
	#define WGINE_ASSERT(x, ...) { if(!(x)) { WGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define WGINE_CORE_ASSERT(x, ...) { if(!(x)) { WGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define WGINE_ASSERT(x, ...)
	#define WGINE_CORE_ASSERT(x, ...)
#endif

//MACROS
#define BIT(x) (1 << x)
#define WGINE_BIND_EVENT_FN(fn) (std::bind(&fn, this, std::placeholders::_1))

#include <memory>
namespace Wgine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> MakeScope(Args &&...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Ref<T> MakeRef(Args &&...args) { return std::make_shared<T>(std::forward<Args>(args)...); }
}
