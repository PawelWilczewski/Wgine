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

	public:
		float GetDeltaSeconds() const { return m_DeltaSeconds; }

	public:
		template <typename T, typename ...Args>
		inline T *ConstructEntity(Args &&...args)
		{
			static_assert(std::is_base_of<Entity, T>::value, "Cannot construct class not deriving from Entity!");
			T *entity = new T(std::forward<Args>(args)...);
			entity->m_Scene = this;
			m_Entities.push_back(entity); // TODO: system where resizing entities happens in increments of 10/20/100 or so?
			if (auto scenEntity = dynamic_cast<SceneEntity *>(entity))
				m_SceneEntities.push_back(scenEntity);
			return entity;
		}

	public:
		std::vector<Entity *> m_Entities;
		std::vector<SceneEntity *> m_SceneEntities; // subset of entities which are sceneentities
		//std::vector<Entity *> m_EventPropagateEntities; // TODO: implement this if performance is bad for looping through all entities
		float m_DeltaSeconds;
		Camera *m_ActiveCamera;
	};
}
