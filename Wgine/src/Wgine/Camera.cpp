#include "WginePCH.h"
#include "Camera.h"

#include <glm/gtx/euler_angles.hpp>

namespace Wgine
{
	void Camera::UpdateEntityMatrix()
	{
		SceneEntity::UpdateEntityMatrix();

		// TODO: this surely isn't optimized (can we just use EntityMatrix, rotate it and inverse it?)
		
		//auto rot = glm::lookAt(glm::vec3(0.f), Transform::VectorForward, Transform::VectorUp);
		//m *= rot;

		//auto m = glm::rotate(glm::mat4(1.f), glm::radians(90.f), Transform::VectorUp);
		//m = glm::rotate(m, glm::radians(90.f), Transform::VectorRight);
		
		auto m = glm::translate(glm::mat4(1.f), m_Transform.Location);

		m = glm::rotate(m, glm::radians(m_Transform.Rotation[2]), GetUpVector()); // yaw rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[1]), GetRightVector()); // pitch rotation
		m = glm::rotate(m, glm::radians(m_Transform.Rotation[0]), GetForwardVector()); // roll rotation
		
		//auto rot = glm::eulerAngleXYZ(glm::radians(m_Transform.Rotation[0]), glm::radians(m_Transform.Rotation[1]), glm::radians(m_Transform.Rotation[2]));
		//m *= rot;


		m = glm::scale(m, m_Transform.Scale);
		

		m_ViewMatrix = glm::inverse(m);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
