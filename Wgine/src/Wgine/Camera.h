#pragma once

#include "Wgine/ECS/Entity.h"

namespace Wgine
{
	class Camera : public SceneEntity
	{
	public:
		Camera()
			: SceneEntity(), m_ProjectionMatrix(glm::mat4(1.f)), m_ViewMatrix(glm::mat4(1.f)), m_ViewProjectionMatrix(glm::mat4(1.f))
		{
			UpdateProjectionMatrix();
		}

		Camera(Transform transform)
			: SceneEntity(transform), m_ProjectionMatrix(glm::mat4(1.f)), m_ViewMatrix(glm::mat4(1.f)), m_ViewProjectionMatrix(glm::mat4(1.f))
		{
			UpdateProjectionMatrix();
		}

	public:
		// Matrices
		const glm::mat4 &GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 &GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void UpdateEntityMatrix() override;
		virtual void UpdateProjectionMatrix() {}

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera()
			: Camera(), m_FOV(45.f), m_NearClip(0.1f), m_FarClip(100000.f), m_Width(1600.f), m_Height(900.f)
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

		PerspectiveCamera(Transform transform, float fovVert = 45.f, float width = 1600.f, float height = 900.f, float nearClip = 0.1f, float farClip = 100000.f)
			: Camera(transform), m_FOV(fovVert), m_Width(width), m_Height(height), m_NearClip(nearClip), m_FarClip(farClip)
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

	public:
		// Projection params
		void SetFOV(const float &fovVert) { m_FOV = fovVert; UpdateProjectionMatrix(); }
		void SetNearClip(const float &nearClip) { m_NearClip = nearClip; UpdateProjectionMatrix(); }
		void SetFarClip(const float &farClip) { m_FarClip = farClip; UpdateProjectionMatrix(); }
		void SetWindowSize(const float &width, const float &height) { m_Width = width; m_Height = height; UpdateProjectionMatrix(); }

	protected:
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_FOV, m_NearClip, m_FarClip, m_Width, m_Height;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera()
			: Camera(), m_Left(-1.f), m_Right(1.f), m_Bottom(-1.f), m_Top(1.f)
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

		OrthographicCamera(Transform transform, float left = -1.f, float right = 1.f, float bottom = -1.f, float top = 1.f)
			: Camera(transform), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

	public:
		// Projection params
		void SetLeft(const float &left) { m_Left = left; UpdateProjectionMatrix(); };
		void SetRight(const float &right) { m_Right = right; UpdateProjectionMatrix(); };
		void SetBottom(const float &bottom) { m_Bottom = bottom; UpdateProjectionMatrix(); };
		void SetUp(const float &top) { m_Top = top; UpdateProjectionMatrix(); };

	protected:
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_Left, m_Right, m_Bottom, m_Top;
	};
}
