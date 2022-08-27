#include "WginePCH.h"
#include "Camera.h"

namespace Wgine
{
	void Camera::UpdateViewMatrix()
	{
		auto view = glm::translate(glm::mat4(1.f), m_Transform.Position);
		view = glm::rotate(view, glm::radians(m_Transform.Rotation[0]), Transform::VectorForward); // roll rotation
		view = glm::rotate(view, glm::radians(m_Transform.Rotation[1] - 90.f), Transform::VectorRight); // pitch rotation
		view = glm::rotate(view, glm::radians(m_Transform.Rotation[2]), Transform::VectorUp); // yaw rotation
		view = glm::scale(view, m_Transform.Scale);

		m_ViewMatrix = glm::inverse(view);

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
