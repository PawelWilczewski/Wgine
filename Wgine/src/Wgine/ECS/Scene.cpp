#include "WginePCH.h"
#include "Scene.h"
#include "Wgine/Camera.h"
#include "Wgine/ECS/Entity.h"


namespace Wgine
{
	Scene::Scene()
		: m_DeltaSeconds (1.f/60.f)
	{
		// TODO: system where resizing entities happens in increments of 10/20/100 or so? is that the standard?
		m_Entities.reserve(50);
	}

	void Scene::OnStart()
	{
		for (const auto &entity : m_Entities)
			entity->OnStart();
	}

	void Scene::OnTick(float deltaSeconds)
	{
		m_DeltaSeconds = deltaSeconds;

		for (const auto &entity : m_Entities)
			entity->OnTick(deltaSeconds);
	}

	void Scene::OnEnd()
	{
		for (auto entity : m_Entities)
			entity->OnEnd();

		for (auto entity : m_Entities)
			delete entity;
	}

	void Scene::OnEvent(Event &e)
	{
		for (auto entity : m_Entities)
			entity->OnEvent(e);
	}
}
