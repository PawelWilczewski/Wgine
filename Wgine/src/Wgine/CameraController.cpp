#include "WginePCH.h"
#include "CameraController.h"
#include "Wgine/Input.h"

namespace Wgine
{
	CameraController::CameraController(Camera *camera, float moveSpeed)
		: m_Camera(camera), m_MoveSpeed(moveSpeed)
	{
	}

	void CameraController::OnStart()
	{
		WGINE_CORE_TRACE("Test from controller");
	}

	void CameraController::OnTick(float deltaSeconds)
	{
		if (Input::IsKeyPressed(WGINE_KEY_W))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetForwardVector() * GetMoveSpeed() * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_S))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetForwardVector() * -GetMoveSpeed() * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_D))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetRightVector() * GetMoveSpeed() * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_A))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetRightVector() * -GetMoveSpeed() * deltaSeconds);
	}

	void CameraController::OnEvent(const Event &e)
	{

	}
}
