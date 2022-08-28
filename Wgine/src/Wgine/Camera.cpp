#include "WginePCH.h"
#include "Camera.h"

#include <glm/gtx/euler_angles.hpp>

namespace Wgine
{
	void Camera::UpdateEntityMatrix()
	{
		SceneEntity::UpdateEntityMatrix();

		// TODO: this surely isn't optimized (can we just use EntityMatrix, rotate it and inverse it?)
				
		auto m = glm::translate(glm::mat4(1.f), m_Transform.Location);
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[2]), GetUpVector()); // yaw rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[1]), GetRightVector()); // pitch rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[0]), GetForwardVector()); // roll rotation
		m = glm::scale(m, m_Transform.Scale);
		
		m_ViewMatrix = glm::inverse(m);
		//m_ViewMatrix = glm::inverse(GetEntityMatrix());
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
