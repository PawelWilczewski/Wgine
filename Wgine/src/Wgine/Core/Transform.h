#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define WGINE_VECTOR_FORWARD (Wgine::Transform::s_VectorForward)
#define WGINE_VECTOR_RIGHT (Wgine::Transform::s_VectorRight)
#define WGINE_VECTOR_UP (Wgine::Transform::s_VectorUp)

namespace Wgine
{
	struct TransformGPU
	{
		TransformGPU(glm::vec3 location = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f))
			: Location(location), Rotation(rotation), Scale(scale)
		{

		}
		alignas(16) glm::vec3 Location;
		alignas(16) glm::vec3 Rotation;
		alignas(16) glm::vec3 Scale;
	};

	class Transform
	{
	public:
		static const glm::vec3 s_VectorForward;
		static const glm::vec3 s_VectorRight;
		static const glm::vec3 s_VectorUp;

	public:
		glm::vec3 Location, Rotation, Scale;

		Transform(const glm::vec3 &location = { 0.f, 0.f, 0.f },
				  const glm::vec3 &rotation = { 0.f, 0.f, 0.f },
				     const glm::vec3 &scale = { 1.f, 1.f, 1.f })
			: Location(location), Rotation(rotation), Scale(scale)
		{}

		const glm::vec3 GetForwardVector() const { return glm::quat(glm::radians(Rotation)) * s_VectorForward; }
		const glm::vec3 GetRightVector() const { return glm::quat(glm::radians(Rotation)) * s_VectorRight; }
		const glm::vec3 GetUpVector() const { return glm::quat(glm::radians(Rotation)) * s_VectorUp; }

		// TODO: what kind of reference should this be? we can't copy it yet just a reference is too weak
		glm::mat4 ToModelMatrix() const
		{
			auto m = glm::translate(glm::mat4(1.f), Location);
			m = glm::rotate(m, glm::radians(Rotation[2]), Transform::s_VectorUp); // yaw rotation
			m = glm::rotate(m, glm::radians(Rotation[1]), Transform::s_VectorRight); // pitch rotation
			m = glm::rotate(m, glm::radians(Rotation[0]), Transform::s_VectorForward); // roll rotation
			return glm::scale(m, Scale);
		}
	};
}
