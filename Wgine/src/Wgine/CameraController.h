#pragma once

#include "Camera.h"
#include "Event/Event.h"

namespace Wgine
{
	class CameraController : Entity
	{
	public:
		CameraController(Camera *camera, float moveSpeed = 5.f);

	protected:
		virtual void OnStart() override;
		virtual void OnTick(float deltaSeconds) override;
		virtual void OnEvent(const Event &e) override;

	private:
		Camera *m_ControlledCamera;
		float m_MoveSpeed;
		float m_MoveSpeedMultiplier = 1.f;
	};
}
