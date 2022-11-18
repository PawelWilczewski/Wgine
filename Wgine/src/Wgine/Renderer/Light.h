#pragma once

#include "Wgine/Core/Transform.h"

namespace Wgine
{
	class Light
	{
	public:
		enum class Type
		{
			None = 0, Point = 1, Spot = 2, Directional = 3, Ambient = 4
		};

	public:
		virtual Type GetLightType() const { return Type::None; } // TODO: should this be = 0 so the class is abstract?

		Light(const glm::vec3 &color = glm::vec3(1.f), float intensity = 1.f)
			: m_Color(color), m_Intensity(intensity)
		{}

		virtual ~Light() {};

		const glm::vec3 &GetColor() const { return m_Color; }
		float GetIntensity() const { return m_Intensity; }

		void SetColor(const glm::vec3 &color) { m_Color = color; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

	protected:
		
		glm::vec3 m_Color;
		float m_Intensity;
	};


	// Ambient light --------------------------------------------------
	class AmbientLight : public Light
	{
	public:
		virtual Type GetLightType() const { return Type::Ambient; }

		AmbientLight(const glm::vec3 &color = glm::vec3(1.f), float intensity = 1.f)
			: Light(color, intensity)
		{}

		virtual ~AmbientLight()
		{}
	};

	struct AmbientLightGPU
	{
		AmbientLightGPU(const AmbientLight &light)
			: Color(light.GetColor()),
			Intensity(light.GetIntensity())
		{}

		alignas(16) glm::vec3 Color;
		float Intensity;
	};


	// Directional light --------------------------------------------------
	class DirectionalLight : public Light
	{
	public:
		virtual Type GetLightType() const override { return Type::Directional; }

		DirectionalLight(const glm::vec3 &rotation = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f), float intensity = 1.f)
			: Light(color, intensity)
		{
			SetRotation(rotation);
		}

		virtual ~DirectionalLight()
		{}

		const glm::vec3 &GetRotation() const { return m_Rotation; }
		const glm::vec3 &GetDirection() const { return m_Direction; }
		void SetRotation(const glm::vec3 &rotation)
		{
			m_Rotation = rotation;
			m_Direction = Transform({ 1.f, 0.f, 0.f }, m_Rotation, { 1.f, 1.f, 1.f }).GetForwardVector();
		}

	protected:
		glm::vec3 m_Rotation;
		glm::vec3 m_Direction;
	};

	struct DirectionalLightGPU
	{
		DirectionalLightGPU(const DirectionalLight &light)
			: Direction(light.GetDirection()),
			Color(light.GetColor()),
			Intensity(light.GetIntensity())
		{}

		alignas(16) glm::vec3 Direction;
		alignas(16) glm::vec3 Color;
		float Intensity;
	};


	// Point light --------------------------------------------------
	class PointLight : public Light
	{
	public:
		virtual Type GetLightType() const override { return Type::Point; }

		PointLight(const glm::vec3 &location = glm::vec3(0.f), glm::vec3 color = glm::vec3(1.f), float intensity = 1.f, float radius = 0.f, float cutoffRadius = 1000.f)
			: Light(color, intensity), m_Location(location), m_Radius(radius), m_CutoffRadius(cutoffRadius)
		{}

		virtual ~PointLight()
		{}

		const glm::vec3 &GetLocation() const { return m_Location; }
		float GetRadius() const { return m_Radius; }
		float GetCutoffRadius() const { return m_CutoffRadius; }

		void SetLocation(const glm::vec3 &location) { m_Location = location; }
		void SetRadius(float radius) { m_Radius = radius; }
		void SetCutoffRadius(float cutoffRadius) { m_CutoffRadius = cutoffRadius; }

	protected:
		glm::vec3 m_Location;
		float m_Radius;
		float m_CutoffRadius;
	};

	struct PointLightGPU
	{
		PointLightGPU(const PointLight &light)
			: Location(light.GetLocation()),
			  Color(light.GetColor()),
			  Intensity(light.GetIntensity()),
			  Radius(light.GetRadius()),
			  CutoffRadius(light.GetCutoffRadius())
		{}

		alignas(16) glm::vec3 Location;
		alignas(16) glm::vec3 Color;
		float Intensity;
		float Radius;
		float CutoffRadius;
	};


	// Spot light --------------------------------------------------
	class SpotLight : public PointLight
	{
	public:
		virtual Type GetLightType() const override { return Type::Spot; }

		SpotLight(const glm::vec3 &location = glm::vec3(0.f),
			const glm::vec3 &rotation = glm::vec3(0.f),
			const glm::vec3 &color = glm::vec3(1.f),
			float intensity = 1.f,
			float radius = 0.f,
			float cutoffRadius = 1000.f,
			float cutoffAngle = 45.f,
			float cutoffAngleInner = 40.f)
			: PointLight(location, color, intensity, radius, cutoffRadius), m_CutoffAngle(cutoffAngle), m_CutoffAngleInner(cutoffAngleInner)
		{
			SetRotation(rotation);
		}

		virtual ~SpotLight()
		{}

		const glm::vec3 &GetRotation() const { return m_Rotation; }
		float GetCutoffAngle() const { return m_CutoffAngle; }
		float GetCutoffAngleInner() const { return m_CutoffAngleInner; }
		const glm::vec3 &GetDirection() const { return m_Direction; }

		void SetRotation(const glm::vec3 &rotation)
		{
			m_Rotation = rotation;
			m_Direction = Transform({ 1.f, 0.f, 0.f }, m_Rotation, { 1.f, 1.f, 1.f }).GetForwardVector();
		}
		void SetCutoffAngle(float cutoffAngle) { m_CutoffAngle = cutoffAngle; }
		void SetCutoffAngleInner(float cutoffAngleInner) { m_CutoffAngleInner = cutoffAngleInner; }

	protected:
		glm::vec3 m_Rotation;
		glm::vec3 m_Direction;
		float m_CutoffAngle;
		float m_CutoffAngleInner;
	};

	struct SpotLightGPU
	{
		SpotLightGPU(const SpotLight &light)
			: Location(light.GetLocation()),
			Direction(light.GetDirection()),
			Color(light.GetColor()),
			Intensity(light.GetIntensity()),
			Radius(light.GetRadius()),
			CutoffRadius(light.GetCutoffRadius()),
			CutoffAngle(light.GetCutoffAngle()),
			CutoffAngleInner(light.GetCutoffAngleInner())
		{}

		alignas(16) glm::vec3 Location;
		alignas(16) glm::vec3 Direction;
		alignas(16) glm::vec3 Color;
		float Intensity;
		float Radius;
		float CutoffRadius;
		float CutoffAngle;
		float CutoffAngleInner;
	};
}
