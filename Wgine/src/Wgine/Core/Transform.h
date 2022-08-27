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
		glm::vec3 Position, Rotation, Scale;

		Transform(const glm::vec3 &position = { 0.f, 0.f, 0.f },
				  const glm::vec3 &rotation = { 0.f, 0.f, 0.f },
				     const glm::vec3 &scale = { 1.f, 1.f, 1.f })
			: Position(position), Rotation(rotation), Scale(scale)
		{}

		const glm::vec3 &GetForwardVector() const { return glm::quat(glm::radians(Rotation)) * VectorForward; }
		const glm::vec3 &GetRightVector() const { return glm::quat(glm::radians(Rotation)) * VectorRight; }
		const glm::vec3 &GetUpVector() const { return glm::quat(glm::radians(Rotation)) * VectorUp; }

		static const glm::vec3 VectorForward;
		static const glm::vec3 VectorRight;
		static const glm::vec3 VectorUp;
	};
}
