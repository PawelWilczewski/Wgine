#pragma once

#include "Transform.h"
#include "Wgine/Renderer/VertexArray.h"
#include "Wgine/Renderer/Shader.h"

namespace Wgine
{
	class Entity
	{
	public:
		// TODO: Add Start(), OnTick(), End()? etc. and ofc add components support etc.
	private:

	};

	class SceneEntity : public Entity
	{
	public:
		SceneEntity()
			: m_Transform(Transform())
		{
			UpdateEntityMatrix();
		}

		SceneEntity(Transform transform)
			: m_Transform(transform)
		{
			UpdateEntityMatrix();
		}

		const Transform &GetTransform() const { return m_Transform; }
		const glm::mat4 &GetEntityMatrix() const { return m_EntityMatrix; }
		const glm::vec3 &GetLocation() const { return m_Transform.Location; }
		const glm::vec3 &GetRotation() const { return m_Transform.Rotation; }
		const glm::vec3 &GetScale() const { return m_Transform.Scale; }
		const glm::vec3 &GetForwardVector() const { return m_Transform.GetForwardVector(); }
		const glm::vec3 &GetRightVector() const { return m_Transform.GetRightVector(); }
		const glm::vec3 &GetUpVector() const { return m_Transform.GetUpVector(); }

		void SetTransform(Transform t) { m_Transform = t; UpdateEntityMatrix(); }
		void SetLocation(glm::vec3 location) { m_Transform.Location = location; UpdateEntityMatrix(); }
		void SetRotation(glm::vec3 rotation) { m_Transform.Rotation = rotation; UpdateEntityMatrix(); }
		void SetScale(glm::vec3 scale) { m_Transform.Scale = scale; UpdateEntityMatrix(); }

		// TEMP
		std::shared_ptr<VertexArray> MeshData;
		std::shared_ptr<Shader> ShaderData;

	protected:
		virtual void UpdateEntityMatrix()
		{
			/*auto m = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), Transform::VectorUp);
			m = glm::rotate(m, glm::radians(90.f), Transform::VectorForward);*/

			auto m = glm::translate(glm::mat4(1.f), m_Transform.Location);
			m = glm::rotate(m, glm::radians(m_Transform.Rotation[0]), Transform::VectorForward); // roll rotation
			m = glm::rotate(m, glm::radians(m_Transform.Rotation[1]), Transform::VectorRight); // pitch rotation
			m = glm::rotate(m, glm::radians(m_Transform.Rotation[2]), Transform::VectorUp); // yaw rotation
			m_EntityMatrix = glm::scale(m, m_Transform.Scale);
		}

	protected:
		Transform m_Transform;
		glm::mat4 m_EntityMatrix;
	};
}
