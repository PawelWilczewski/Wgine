#include "WginePCH.h"
#include "CameraController.h"

namespace Wgine
{
	CameraController::CameraController(Camera *camera, float moveSpeed)
	{
	}

	void CameraController::OnStart()
	{
		WGINE_CORE_TRACE("Test from controller");
	}

	void CameraController::OnTick(float deltaSeconds)
	{

	}

	void CameraController::OnEvent(const Event &e)
	{

	}
}
