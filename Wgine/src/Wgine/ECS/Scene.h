#pragma once

#include <vector>
#include "Wgine/ECS/Entity.h"
#include "Wgine/Event/Event.h"
#include "Wgine/Camera/Camera.h"
#include <glm/glm.hpp>

namespace Wgine
{
	class Scene
	{
		friend class Renderer;
	public:
		Scene();

	public:
		void OnStart();
		void OnTick(float deltaSeconds);
		void OnEnd();
		void OnEvent(Event &e);

		void SetActiveCamera(Camera *camera) { m_ActiveCamera = camera; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ActiveCamera->GetViewProjectionMatrix(); }
		const Camera *GetActiveCamera() const { return m_ActiveCamera; }

	public:
		float GetDeltaSeconds() const { return m_DeltaSeconds; }

	public:
		template <typename T, typename ...Args>
		inline T *ConstructEntity(Args &&...args)
		{
			static_assert(std::is_base_of<Entity, T>::value, "Cannot construct class not deriving from Entity!"); // TODO: when switched c++ 20 use constraints and concepts? https://en.cppreference.com/w/cpp/language/constraints
			T *entity = new T(std::forward<Args>(args)...); // TODO: wtf? shouldnt we use unique pointers here instead?
			entity->m_Scene = this;
			m_Entities.push_back(entity);
			if (auto sceneEntity = dynamic_cast<SceneEntity *>(entity))
				m_SceneEntities.push_back(sceneEntity);
			return entity;
		}

		template <typename T, typename ...Args>
		inline T *ConstructLight(Args &&...args)
		{
			static_assert(std::is_base_of<Light, T>::value, "Cannot construct class not deriving from Light!"); // TODO: when switched c++ 20 use constraints and concepts? https://en.cppreference.com/w/cpp/language/constraints
			T *light = new T(std::forward<Args>(args)...); // TODO: wtf? shouldnt we use unique pointers here instead?
			m_Lights.push_back(light);
			return light;
		}

	public:
		std::vector<Entity *> m_Entities;
		std::vector<SceneEntity *> m_SceneEntities; // subset of entities which are sceneentities
		std::vector<class Light *> m_Lights;
		//std::vector<Entity *> m_EventPropagateEntities; // TODO: implement this if performance is bad for looping through all entities
		float m_DeltaSeconds;
		Camera *m_ActiveCamera;
	};
}
