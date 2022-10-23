#include "WginePCH.h"
#include "Mesh.h"
#include "Wgine/Renderer/RendererDebug.h"

namespace Wgine
{
	static float angleBetweenVectors(const glm::vec3 &v1, const glm::vec3 &v2) { return glm::acos(glm::dot(glm::normalize(v1), (glm::normalize(v2)))); }

	void Mesh::RecalculateNormals()
	{
		// area and angle-weighted smooth shading
		for (int i = 0; i < m_Indices.size(); i += 3)
		{
			auto v10 = m_Vertices[m_Indices[i + 1]].Position - m_Vertices[m_Indices[i]].Position;
			auto v20 = m_Vertices[m_Indices[i + 2]].Position - m_Vertices[m_Indices[i]].Position;
			auto v21 = m_Vertices[m_Indices[i + 2]].Position - m_Vertices[m_Indices[i + 1]].Position;
			auto cross = glm::cross(v20, v10);
			auto normal = glm::normalize(cross);
			auto area = 0.5f * glm::length(cross);
			auto angle0 = angleBetweenVectors(v10, v20);
			auto angle1 = angleBetweenVectors(v21, -v10);
			auto angle2 = angleBetweenVectors(-v20, -v21);
				
			m_Vertices[m_Indices[i]].Normal += normal * area * angle0;
			m_Vertices[m_Indices[i + 1]].Normal += normal * area * angle1;
			m_Vertices[m_Indices[i + 2]].Normal += normal * area * angle2;
		}

		for (auto &vert : m_Vertices)
			vert.Normal = glm::normalize(vert.Normal);
	}
}
