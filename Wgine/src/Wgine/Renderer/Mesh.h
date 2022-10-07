#pragma once

#include <glm/glm.hpp>
#include <Wgine/Core/Core.h>
#include "Wgine/Renderer/Vertex.h"

namespace Wgine
{
	class Mesh
	{
		// TODO: for performance: mark as dirty if something changed? only if dirty update ibo and vbo
	public:
		Mesh()
			: m_Vertices()
		{}

		Mesh(Vertex *triVertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount)
			: m_Vertices(triVertices, triVertices + verticesCount), m_Indices(indices, indices + indicesCount)
		{}

		const std::vector<Vertex> GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t> GetIndices() const { return m_Indices; }

		std::vector<Vertex> GetVerticesTransformed(const glm::mat4 &t) const
		{
			auto result = std::vector<Vertex>();
			result.resize(m_Vertices.size());
			for (int i = 0; i < m_Vertices.size(); i++)
				result[i] = Vertex(glm::vec3(t * glm::vec4(m_Vertices[i].Position, 1.f)), m_Vertices[i].Color, m_Vertices[i].TexCoord);
			return result;
		}

		std::vector<uint32_t> GetIndicesOffset(uint32_t offset) const
		{
			auto result = std::vector<uint32_t>();
			result.resize(m_Indices.size());
			//WGINE_CORE_TRACE("Get offset original/offset:");
			for (int i = 0; i < m_Indices.size(); i++)
			{
				result[i] = m_Indices[i] + offset;
				//WGINE_CORE_TRACE("\t{0}/{1}:", m_Indices[i], result[i]);
			}
			return result;
		}
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
		void AddIndex(uint32_t i) // TODO: add indices by ptr and count
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
			m_Vertices.reserve(m_Vertices.size() + 3);
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2 });

			m_Indices.reserve(m_Indices.size() + 3);
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2 });
		}
		void AddQuad(Vertex v0, Vertex v1, Vertex v2, Vertex v3)
		{
			uint32_t nextIndex = m_Vertices.size();
			m_Vertices.reserve(m_Vertices.size() + 4);
			m_Vertices.insert(m_Vertices.end(), { v0, v1, v2, v3 });

			m_Indices.reserve(m_Indices.size() + 6);
			m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2, nextIndex + 2, nextIndex + 3, nextIndex });
		}

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
