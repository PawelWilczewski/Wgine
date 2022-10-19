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

	public:
		void AddVertex(Vertex v) { m_Vertices.push_back(v); }
		void AddVertices(std::vector<Vertex> v)
		{
			m_Vertices.insert(m_Vertices.end(), v.begin(), v.end());
		}
		void AddVertices(Vertex *v, int count)
		{
			m_Vertices.insert(m_Vertices.end(), v, v + count);
		}
		void AddIndex(uint32_t i)
		{
			m_Indices.push_back(i);
		}
		void AddIndices(uint32_t *i, int count)
		{
			m_Indices.insert(m_Indices.end(), i, i + count);
		}
		void AddIndices(std::vector<uint32_t> i)
		{
			m_Indices.insert(m_Indices.end(), i.begin(), i.end());
		}
		void AddTriangle(Vertex v0, Vertex v1, Vertex v2)
		{
			uint32_t nextIndex = m_Vertices.size();
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2 });
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2 });
		}
		void AddQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
		{
			uint32_t nextIndex = m_Vertices.size();
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2, v3 });
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2, nextIndex + 2, nextIndex + 3, nextIndex });
		}

		void RecalculateNormals()
		{
			Scope<float[]> counts(new float[m_Vertices.size()]);

			for (int i = 0; i < m_Indices.size(); i += 3)
			{
				auto first = m_Indices[i];
				auto second = m_Indices[i + 1];
				auto third = m_Indices[i + 2];
				auto firstNormal = glm::cross(m_Vertices[third].Position - m_Vertices[first].Position, m_Vertices[second].Position - m_Vertices[first].Position);
				auto secondNormal = glm::cross(m_Vertices[first].Position - m_Vertices[second].Position, m_Vertices[third].Position - m_Vertices[second].Position);
				auto thirdNormal = glm::cross(m_Vertices[first].Position - m_Vertices[third].Position, m_Vertices[second].Position - m_Vertices[third].Position);
				
				auto area = 0.5f * secondNormal.length();

				m_Vertices[first].Normal += glm::normalize(firstNormal) * area;
				m_Vertices[second].Normal += glm::normalize(firstNormal) * area;
				m_Vertices[third].Normal += glm::normalize(firstNormal) * area;
				
				// TODO: maybe weighted normal by adding face
				counts[first] += area;
				counts[second] += area;
				counts[third] += area;
			}

			for (int i = 0; i < m_Vertices.size(); i++)
				m_Vertices[i].Normal /= counts[i];
		}

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
