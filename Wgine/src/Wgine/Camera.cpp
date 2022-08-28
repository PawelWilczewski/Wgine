#include "WginePCH.h"
#include "Camera.h"

namespace Wgine
{
	void Camera::UpdateEntityMatrix()
	{
		SceneEntity::UpdateEntityMatrix();

		// TODO: this surely isn't optimized (can we just use EntityMatrix, rotate it and inverse it?)
		auto m = glm::translate(glm::mat4(1.f), m_Transform.Location);
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[0]), Transform::VectorForward); // roll rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[1] - 90.f), Transform::VectorRight); // pitch rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[2]), Transform::VectorUp); // yaw rotation
		m = glm::scale(m, m_Transform.Scale);

		m_ViewMatrix = glm::inverse(m);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
