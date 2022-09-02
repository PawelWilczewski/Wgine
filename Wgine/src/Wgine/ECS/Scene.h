#pragma once

#include <vector>
#include "Wgine/ECS/Entity.h"
#include "Wgine/Event/Event.h"

namespace Wgine
{
	class Scene
	{
	public:
		Scene();

	public:
		void OnStart();
		void OnTick(float deltaSeconds);
		void OnEnd();
		void OnEvent(Event &e);

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
			return entity;
		}

	private:
		std::vector<Entity *> m_Entities;
		//std::vector<Entity *> m_EventPropagateEntities; // TODO: implement this if performance is bad for looping through all entities
		float m_DeltaSeconds;
	};
}
