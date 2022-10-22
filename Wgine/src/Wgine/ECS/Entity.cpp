#include "WginePCH.h"
#include "Entity.h"
#include "Scene.h"

#include "Wgine/Renderer/RendererDebug.h"

namespace Wgine
{
	void SceneEntity::DebugDrawNormals(float size) const
	{
		for (const auto &vert : MeshData->GetVertices())
			RendererDebug::DrawLine(m_EntityMatrix * glm::vec4(vert.Position, 1.f), m_EntityMatrix * glm::vec4(vert.Position + vert.Normal * size, 1.f));
	}
}
