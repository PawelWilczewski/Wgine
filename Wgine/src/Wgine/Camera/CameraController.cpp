#include "WginePCH.h"
#include "CameraController.h"
#include "Wgine/Core/Input.h"
#include "Wgine/Core/Application.h"

namespace Wgine
{
	CameraController::CameraController(Camera *camera, float moveSpeed, float speedMultiplierDelta, float minSpeed, float maxSpeed)
		: m_Camera(camera), m_MoveSpeed(moveSpeed), m_MoveSpeedMultiplierDelta(speedMultiplierDelta), m_MinSpeed(minSpeed), m_MaxSpeed(maxSpeed)
	{
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

	void CameraController::OnEvent(Event &e)
	{
		EventDispatcher ed(e);
		ed.Dispatch<MouseMovedEvent>(WGINE_BIND_EVENT_FN(CameraController::OnMouseMoved));
		ed.Dispatch<MouseButtonPressedEvent>(WGINE_BIND_EVENT_FN(CameraController::OnMouseButtonPressed));
		ed.Dispatch<MouseScrolledEvent>(WGINE_BIND_EVENT_FN(CameraController::OnMouseScrolled));
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent &e)
	{
		if (Application::Get().GetWindow().GetShowMouse()) // TODO: obv better api and this is blatantly wrong??? shouldn't be negation
			return false;
		
		auto pos = e.GetPosition();
		static bool initial = true;
		if (initial)
		{
			m_LastMousePosition = pos;
			initial = false;
		}
		auto delta = pos - m_LastMousePosition;
		m_LastMousePosition = pos;

		// TODO: should we do something like this to ensure the same rotation always if the same physical movement was performed? regardless of the current res
		//auto deltaNormalized = delta / glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		m_Camera->SetRotation(m_Camera->GetRotation() + glm::vec3(0.f, delta.y, delta.x) * 0.05f);

		return false;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent &e)
	{
		auto window = &Application::Get().GetWindow();
		if (e.GetMouseButton() == WGINE_MOUSE_BUTTON_RIGHT)
			window->SetShowMouse(!window->GetShowMouse());
		return false;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent &e)
	{
		m_MoveSpeedMultiplier += e.GetOffsetY() * m_MoveSpeedMultiplierDelta * glm::pow(m_MoveSpeedMultiplier, 1.01f);
		m_MoveSpeedMultiplier = glm::clamp(m_MoveSpeedMultiplier, m_MinSpeed, m_MaxSpeed);
		return false;
	}
}
