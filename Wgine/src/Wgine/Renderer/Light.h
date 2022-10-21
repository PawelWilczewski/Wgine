#pragma once

#include "Wgine/Core/Transform.h"

namespace Wgine
{
	class Light
	{
	public:
		enum class LightType
		{
			None = 0, PointLight = 1
		};

	public:
		virtual LightType GetLightType() const { return LightType::None; }

		Light() {}
		Light(const Transform &t, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f))
			: m_Transform(t), m_Intensity(intensity), m_Color(color)
		{}

		virtual ~Light() {}; // = 0; TODO: how to achieve abstract class without linking errors?

		// TODO: in the future this should be a part of transform component so this class is simplified and the actual ECS will improve performance
		float GetIntensity() const { return m_Intensity; }
		const glm::vec3 &GetColor() const { return m_Color; }
		const Transform &GetTransform() const { return m_Transform; }
		const glm::vec3 &GetLocation() const { return m_Transform.Location; }
		// TODO: i.e. point light doesn't need rotation nor scale; we should implement some other class like DirectionalLight and that should include those (Area light/sun light etc.)
		const glm::vec3 &GetRotation() const { return m_Transform.Rotation; }
		const glm::vec3 &GetScale() const { return m_Transform.Scale; }
		const glm::vec3 GetForwardVector() const { return m_Transform.GetForwardVector(); }
		const glm::vec3 GetRightVector() const { return m_Transform.GetRightVector(); }
		const glm::vec3 GetUpVector() const { return m_Transform.GetUpVector(); }

		void SetIntensity(float intensity) { m_Intensity = intensity; }
		void SetColor(const glm::vec3 &color) { m_Color = color; }
		void SetTransform(const Transform &t) { m_Transform = t; }
		void SetLocation(const glm::vec3 &location) { m_Transform.Location = location; }
		void SetRotation(const glm::vec3 &rotation) { m_Transform.Rotation = rotation; }
		void SetScale(const glm::vec3 &scale) { m_Transform.Scale = scale; }

	protected:
		float m_Intensity = 1.f;
		glm::vec3 m_Color = glm::vec3(1.f);
		Transform m_Transform;
	};

	class PointLight : public Light
	{
	public:
		virtual LightType GetLightType() const override { return LightType::PointLight; }

		PointLight()
		{
		}

		PointLight(const Transform &t, float intensity = 1.f, glm::vec3 color = glm::vec3(1.f))
			: Light(t, intensity, color)
		{
		}

		virtual ~PointLight()
		{}
	};

	struct PointLightGPU
	{
		PointLightGPU(const PointLight &pointLight)
			: Location(pointLight.GetLocation()),
			  Rotation(pointLight.GetRotation()),
			  Scale(pointLight.GetScale()),
			  Intensity(pointLight.GetIntensity()),
			  Color(pointLight.GetColor())
		{}

		alignas(16) glm::vec3 Location;
		alignas(16) glm::vec3 Rotation;
		alignas(16) glm::vec3 Scale;
		float Intensity;
		alignas(16) glm::vec3 Color;
	};
}
