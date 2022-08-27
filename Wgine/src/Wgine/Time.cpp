#include "WginePCH.h"
#include "Time.h"

namespace Wgine
{
	const std::chrono::steady_clock::time_point Time::s_StartTime = std::chrono::high_resolution_clock::now();

	std::chrono::steady_clock::time_point Time::s_LastFrameTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point Time::s_CurrentFrameTime = std::chrono::high_resolution_clock::now();
}
