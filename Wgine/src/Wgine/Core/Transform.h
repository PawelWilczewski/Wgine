#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define WGINE_VECTOR_FORWARD (Wgine::Transform::VectorForward)
#define WGINE_VECTOR_RIGHT (Wgine::Transform::VectorRight)
#define WGINE_VECTOR_UP (Wgine::Transform::VectorUp)

namespace Wgine
{
	class Transform
	{
	public:
		glm::vec3 Location, Rotation, Scale;

		Transform(const glm::vec3 &location = { 0.f, 0.f, 0.f },
				  const glm::vec3 &rotation = { 0.f, 0.f, 0.f },
				     const glm::vec3 &scale = { 1.f, 1.f, 1.f })
			: Location(location), Rotation(rotation), Scale(scale)
		{}

		const glm::vec3 &GetForwardVector() const { return glm::quat(glm::radians(Rotation)) * VectorForward; }
		const glm::vec3 &GetRightVector() const { return glm::quat(glm::radians(Rotation)) * VectorRight; }
		const glm::vec3 &GetUpVector() const { return glm::quat(glm::radians(Rotation)) * VectorUp; }

		glm::mat4 ToModelMatrix() const
		{
			auto m = glm::translate(glm::mat4(1.f), Location);
			m = glm::rotate(m, glm::radians(Rotation[2]), Transform::VectorUp); // yaw rotation
			m = glm::rotate(m, glm::radians(Rotation[1]), Transform::VectorRight); // pitch rotation
			m = glm::rotate(m, glm::radians(Rotation[0]), Transform::VectorForward); // roll rotation
			return glm::scale(m, Scale);
		}

		static const glm::vec3 VectorForward;
		static const glm::vec3 VectorRight;
		static const glm::vec3 VectorUp;
	};
}
