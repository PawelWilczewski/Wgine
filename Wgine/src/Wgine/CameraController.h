#pragma once

#include "Camera.h"
#include "Event/Event.h"
#include "Event/MouseEvent.h"

#include "glm/glm.hpp"

namespace Wgine
{
	class CameraController : public Entity
	{
	public:
		CameraController(Camera *camera, float moveSpeed = 5.f, float speedMultiplierDelta = 0.2f, float minSpeed = 0.1f, float maxSpeed = 10000.f);

		inline float GetMoveSpeed() const { return m_MoveSpeed * m_MoveSpeedMultiplier; }

	protected:
		virtual void OnStart() override;
		virtual void OnTick(float deltaSeconds) override;
		virtual void OnEvent(Event &e) override;

		bool OnMouseMoved(MouseMovedEvent &e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent &e);
		bool OnMouseScrolled(MouseScrolledEvent &e);

	private:
		Camera *m_Camera;
		float m_MoveSpeed;
		float m_MoveSpeedMultiplier = 1.f;
		float m_MoveSpeedMultiplierDelta = 0.2f;
		float m_MaxSpeed = 10000.f;
		float m_MinSpeed = 0.1f;
		glm::vec2 m_LastMousePosition = glm::vec2(0.f);
	};
}
