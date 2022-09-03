#pragma once

#include "Wgine/ECS/Entity.h"
#include "Wgine/Event/ApplicationEvent.h"

namespace Wgine
{
	class Camera : public SceneEntity
	{
	public:
		Camera()
			: m_ProjectionMatrix(glm::mat4(1.f)), m_ViewMatrix(glm::mat4(1.f)), m_ViewProjectionMatrix(glm::mat4(1.f))
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

		Camera(Transform transform)
			: SceneEntity(transform), m_ProjectionMatrix(glm::mat4(1.f)), m_ViewMatrix(glm::mat4(1.f)), m_ViewProjectionMatrix(glm::mat4(1.f))
		{
			UpdateProjectionMatrix();
			UpdateEntityMatrix();
		}

	public:
		// Matrices
		const glm::mat4 &GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4 &GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	protected:
		virtual void OnStart() override;
		virtual void OnEvent(Event &e) override;

		virtual void UpdateEntityMatrix() override;
		virtual void UpdateProjectionMatrix() { RecalculateViewProjectionMatrix(); }
		inline void RecalculateViewProjectionMatrix() { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }
		virtual bool OnWindowResized(WindowResizeEvent &e);
		virtual void UpdateWindowSize(float width, float height) = 0;

	protected:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera()
			: m_FOV(45.f), m_NearClip(0.1f), m_FarClip(100000.f), m_Width(1600.f), m_Height(900.f)
		{
		}

		PerspectiveCamera(Transform transform, float fovVert = 45.f, float nearClip = -100000.f, float farClip = 100000.f, float width = 1600.f, float height = 900.f)
			: Camera(transform), m_FOV(fovVert), m_Width(width), m_Height(height), m_NearClip(nearClip), m_FarClip(farClip)
		{
		}

	public:
		// Projection params
		void SetFOV(const float &fovVert) { m_FOV = fovVert; UpdateProjectionMatrix(); }
		void SetNearClip(const float &nearClip) { m_NearClip = nearClip; UpdateProjectionMatrix(); }
		void SetFarClip(const float &farClip) { m_FarClip = farClip; UpdateProjectionMatrix(); }
		void SetWindowSize(const float &width, const float &height) { m_Width = width; m_Height = height; UpdateProjectionMatrix(); }

	protected:
		virtual void UpdateWindowSize(float width, float height) override;
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_FOV, m_NearClip, m_FarClip, m_Width, m_Height;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera()
			: m_Left(-1.f), m_Right(1.f), m_Bottom(-1.f), m_Top(1.f), m_NearClip(-0.1f), m_FarClip(10000.f), m_HalfSize(20.f/2)
		{
		}

		OrthographicCamera(Transform transform, float verticalSize = 20.f,  float nearClip = -0.1f, float farClip = 10000.f,
			float left = -1.f, float right = 1.f, float bottom = -1.f, float top = 1.f)
			: Camera(transform), m_HalfSize(verticalSize / 2), m_NearClip(nearClip), m_FarClip(farClip), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
		{
		}

	public:
		// Projection params
		void SetLeft(const float &left) { m_Left = left; UpdateProjectionMatrix(); };
		void SetRight(const float &right) { m_Right = right; UpdateProjectionMatrix(); };
		void SetBottom(const float &bottom) { m_Bottom = bottom; UpdateProjectionMatrix(); };
		void SetUp(const float &top) { m_Top = top; UpdateProjectionMatrix(); };
		void SetNearClip(const float &nearClip) { m_NearClip = nearClip; UpdateProjectionMatrix(); }
		void SetFarClip(const float &farClip) { m_FarClip = farClip; UpdateProjectionMatrix(); }

	protected:
		virtual void UpdateWindowSize(float width, float height) override;
		virtual void UpdateProjectionMatrix() override;

	private:
		float m_Left, m_Right, m_Bottom, m_Top, m_NearClip, m_FarClip, m_HalfSize;
	};
}
