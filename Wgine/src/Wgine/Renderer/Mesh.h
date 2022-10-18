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
			: m_Vertices()
		{}

		Mesh(Vertex *triVertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount)
			: m_Vertices(triVertices, triVertices + verticesCount), m_Indices(indices, indices + indicesCount)
		{}

		const std::vector<Vertex> &GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t> &GetIndices() const { return m_Indices; }

		std::vector<Vertex> GetVerticesTransformed(const glm::mat4 &t) const
		{
			auto result = std::vector<Vertex>();
			result.reserve(m_Vertices.size());
			for (int i = 0; i < m_Vertices.size(); i++)
				result.push_back(Vertex(glm::vec3(t * glm::vec4(m_Vertices[i].Position, 1.f)), m_Vertices[i].Color, m_Vertices[i].TexCoord));
			return result;
		}

		void PasteVerticesTransformed(Vertex *dst, const glm::mat4 &t) const
		{
			for (int i = 0; i < m_Vertices.size(); i++)
				*(dst + i) = { glm::vec3(t * glm::vec4(m_Vertices[i].Position, 1.f)), m_Vertices[i].Color, m_Vertices[i].TexCoord };
		}

		std::vector<uint32_t> GetIndicesOffset(uint32_t offset) const
		{
			auto result = std::vector<uint32_t>();
			result.reserve(m_Indices.size());
			for (int i = 0; i < m_Indices.size(); i++)
				result.push_back(m_Indices[i] + offset);
			return result;
		}

		void PasteIndicesOffset(uint32_t *dst, uint32_t offset) const
		{
			for (int i = 0; i < m_Indices.size(); i++)
				*(dst + i) = m_Indices[i] + offset;
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

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
