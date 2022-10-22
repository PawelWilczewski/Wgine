#include "WginePCH.h"
#include "Mesh.h"
#include "Wgine/Renderer/RendererDebug.h"

namespace Wgine
{
	static float angleBetweenVectors(const glm::vec3 &v1, const glm::vec3 &v2) { return glm::acos(glm::dot(glm::normalize(v1), (glm::normalize(v2)))); }

	void Mesh::RecalculateNormals()
	{
		// source: https://www.bytehazard.com/articles/vertnorm.html

		// for each face a
		for (int i = 0; i < m_Indices.size(); i += 3)
		{
			Vertex &av0 = m_Vertices[m_Indices[i]];
			Vertex &av1 = m_Vertices[m_Indices[i + 1]];
			Vertex &av2 = m_Vertices[m_Indices[i + 2]];

			auto aCross = glm::cross(
				av1.Position - av0.Position,
				av1.Position - av2.Position
			);
			auto aArea = glm::length(aCross) * 0.5f;
			auto aNormal = glm::vec3(0.f);// glm::normalize(aCross) *aArea;

			// for each vert of a
			for (int av = i; av < i + 3; av++)
			{
				// for each face b
				for (int j = 0; j < m_Indices.size(); j += 3)
				{
					// skip if self
					if (j == i) continue;

					Vertex &bv0 = m_Vertices[m_Indices[j]];
					Vertex &bv1 = m_Vertices[m_Indices[j + 1]];
					Vertex &bv2 = m_Vertices[m_Indices[j + 2]];

					// b normal
					auto bCross = glm::cross(
						bv1.Position - bv0.Position,
						bv1.Position - bv2.Position
					);
					auto bNormal = glm::normalize(bCross);
					auto bArea = glm::length(bCross) * 0.5f;

					// a and b share av0
					if (m_Indices[i] == m_Indices[j] || m_Indices[i] == m_Indices[j + 1] || m_Indices[i] == m_Indices[j + 2])
					{
						auto angle = angleBetweenVectors(av0.Position - av1.Position, av0.Position - av2.Position);
						aNormal += bNormal * bArea * angle;
					}

					// a and b share av1
					if (m_Indices[i + 1] == m_Indices[j] || m_Indices[i + 1] == m_Indices[j + 1] || m_Indices[i + 1] == m_Indices[j + 2])
					{
						auto angle = angleBetweenVectors(av1.Position - av0.Position, av1.Position - av2.Position);
						aNormal += bNormal * bArea * angle;
					}

					// a and b share av2
					if (m_Indices[i + 2] == m_Indices[j] || m_Indices[i + 2] == m_Indices[j + 1] || m_Indices[i + 2] == m_Indices[j + 2])
					{
						auto angle = angleBetweenVectors(av2.Position - av0.Position, av2.Position - av1.Position);
						aNormal += bNormal * bArea * angle;
					}
				}

				m_Vertices[m_Indices[av]].Normal = normalize(aNormal);
			}
		}
	}

	// must not share vertices

	void Mesh::RecalculateNormalsFlat()
	{
		// for each face a
		for (int i = 0; i < m_Indices.size(); i += 3)
		{
			auto n = glm::normalize(glm::cross(m_Vertices[m_Indices[i]].Position - m_Vertices[m_Indices[i + 2]].Position, m_Vertices[m_Indices[i]].Position - m_Vertices[m_Indices[i + 1]].Position));
			m_Vertices[m_Indices[i]].Normal = n;
			m_Vertices[m_Indices[i + 1]].Normal = n;
			m_Vertices[m_Indices[i + 2]].Normal = n;
		}
	}
}
