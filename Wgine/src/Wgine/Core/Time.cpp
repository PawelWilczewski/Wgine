#include "WginePCH.h"
#include "Time.h"

namespace Wgine
{
	// this should be initialized regardless, but doing it here so there's no issue with using Time without Init (foolproof)
	std::chrono::steady_clock::time_point Time::s_StartTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point Time::s_LastFrameTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point Time::s_CurrentFrameTime = std::chrono::high_resolution_clock::now();
}
