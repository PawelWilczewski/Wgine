#pragma once

#include "Camera.h"
#include "Event/Event.h"

namespace Wgine
{
	class CameraController : Entity
	{
	public:
		CameraController(Camera *camera, float moveSpeed = 5.f);

		inline float GetMoveSpeed() const { return m_MoveSpeed * m_MoveSpeedMultiplier; }

	protected:
		virtual void OnStart() override;
		virtual void OnTick(float deltaSeconds) override;
		virtual void OnEvent(const Event &e) override;

	private:
		Camera *m_Camera;
		float m_MoveSpeed;
		float m_MoveSpeedMultiplier = 1.f;
	};
}
