#pragma once

#include <glm/glm.hpp>
#include <Wgine/Core/Core.h>
#include "Wgine/Renderer/Vertex.h"

namespace Wgine
{
	class Mesh
	{
	public:
		Mesh()
			: m_Vertices(), m_Indices()
		{}

		Mesh(Vertex *triVertices, uint32_t verticesCount, uint32_t *indices, uint32_t indicesCount, bool recalculateNormals = false)
			: m_Vertices(triVertices, triVertices + verticesCount), m_Indices(indices, indices + indicesCount)
		{
			if (recalculateNormals) RecalculateNormals();
		}

		// TODO: wgine asset file path wrapper
		// TODO: probs move to MeshLibrary
		// TODO: import options like which axis up, scale for each axis (i. e. -1 to mirror) etc.
		Mesh(const std::string &filePath, bool recalculateNormals = false);

		virtual ~Mesh() {}

	public:
		const std::vector<Vertex> &GetVertices() const { return m_Vertices; }
		const std::vector<uint32_t> &GetIndices() const { return m_Indices; }

		// Editing mesh
		void AddVertex(const Vertex &v);
		void AddVertices(std::vector<Vertex> v);
		void AddVertices(Vertex *v, int count);
		void AddIndex(uint32_t i);
		void AddIndices(uint32_t *i, int count);
		void AddIndices(std::vector<uint32_t> i);
		void AddTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2);
		void AddQuad(const Vertex &v0, const Vertex &v1, Vertex v2, Vertex v3);

		// must share vertices for smooth and must not for flat
		void RecalculateNormals();

		bool HasDirtyVertices() const { return m_DirtyVertices; }

		//void ClearDirtyIndicesFlag() { m_DirtyIndices = false; }
		void ClearDirtyVerticesFlag() { m_DirtyVertices = false; }

	protected:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
		bool m_DirtyIndices = true; // TODO: what to do about this...
		bool m_DirtyVertices = true;
	};
}
