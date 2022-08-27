#pragma once

#include <chrono>

namespace Wgine
{
	class Time
	{
	public:
		static void Init()
		{
			s_StartTime = std::chrono::high_resolution_clock::now();
			s_LastFrameTime = s_StartTime;
			s_CurrentFrameTime = s_StartTime;
		}

		static float GetTimeSeconds()
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - s_StartTime).count() / 1000000000.f;
		}

		static float GetDeltaSeconds()
		{
			auto delta = std::chrono::duration_cast<std::chrono::nanoseconds>(s_CurrentFrameTime - s_LastFrameTime).count() / 1000000000.f;
			return delta < 0.000001f ? 1.f/60.f : delta;
		}

		static void FrameBegin()
		{
			s_LastFrameTime = s_CurrentFrameTime;
			s_CurrentFrameTime = std::chrono::high_resolution_clock::now();
		}

	private:
		static std::chrono::steady_clock::time_point s_StartTime;
		static std::chrono::steady_clock::time_point s_LastFrameTime;
		static std::chrono::steady_clock::time_point s_CurrentFrameTime;
	};
}
