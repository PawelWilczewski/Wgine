#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Wgine {
	class WGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core macros
#define WGINE_CORE_TRACE(...) ::Wgine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WGINE_CORE_INFO(...)  ::Wgine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WGINE_CORE_WARN(...)  ::Wgine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WGINE_CORE_ERROR(...) ::Wgine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define WGINE_CORE_FATAL(...) ::Wgine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client macros
#define WGINE_TRACE(...) ::Wgine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WGINE_INFO(...)  ::Wgine::Log::GetClientLogger()->info(__VA_ARGS__)
#define WGINE_WARN(...)  ::Wgine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WGINE_ERROR(...) ::Wgine::Log::GetClientLogger()->error(__VA_ARGS__)
#define WGINE_FATAL(...) ::Wgine::Log::GetClientLogger()->fatal(__VA_ARGS__)
