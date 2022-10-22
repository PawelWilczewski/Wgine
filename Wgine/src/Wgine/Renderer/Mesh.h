#pragma once

#include <glm/glm.hpp>
#include <Wgine/Core/Core.h>
#include "Wgine/Renderer/Vertex.h"

namespace Wgine
{
	// TODO: for performance: mark as dirty if something changed? only if dirty update ibo and vbo
	class Mesh
	{
	public:
		Mesh()
			: m_Vertices(), m_Indices()
		{}

		Mesh(Vertex *triVertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount)
			: m_Vertices(triVertices, triVertices + verticesCount), m_Indices(indices, indices + indicesCount)
		{
			RecalculateNormals();
		}

		const std::vector<Vertex> &GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t> &GetIndices() const { return m_Indices; }

		void AddVertex(const Vertex &v) { m_Vertices.push_back(v); }
		void AddVertices(std::vector<Vertex> v) { m_Vertices.insert(m_Vertices.end(), v.begin(), v.end()); }
		void AddVertices(Vertex *v, int count) { m_Vertices.insert(m_Vertices.end(), v, v + count); }
		void AddIndex(uint32_t i) { m_Indices.push_back(i); }
		void AddIndices(uint32_t *i, int count) { m_Indices.insert(m_Indices.end(), i, i + count); }
		void AddIndices(std::vector<uint32_t> i) { m_Indices.insert(m_Indices.end(), i.begin(), i.end()); }
		void AddTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2)
		{
			uint32_t nextIndex = m_Vertices.size();
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2 });
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2 });
		}
		void AddQuad(const Vertex &v0, const Vertex &v1, Vertex v2, Vertex v3)
		{
			uint32_t nextIndex = m_Vertices.size();
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2, v3 });
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2, nextIndex + 2, nextIndex + 3, nextIndex });
		}

		static float angleBetweenVectors(const glm::vec3 &v1, const glm::vec3 &v2) { return glm::acos(glm::dot(glm::normalize(v1), (glm::normalize(v2)))); }

		void RecalculateNormals(/*float maxAngleDegrees = 60.f*/)
		{
			// source: https://www.bytehazard.com/articles/vertnorm.html

			//float radians = glm::radians(maxAngleDegrees);

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
				auto aNormal = glm::normalize(aCross) * aArea;

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
		void RecalculateNormalsFlat()
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

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
