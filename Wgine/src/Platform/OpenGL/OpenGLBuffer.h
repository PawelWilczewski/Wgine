#pragma once

#include "Wgine/Renderer/Buffer.h"

#include <glad/glad.h>
#include "Wgine/Renderer/Mesh.h"

namespace Wgine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t typeSize, uint32_t count);
		OpenGLVertexBuffer(float *vertices, uint32_t typeSize, uint32_t count);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void PrintDebug(int indent) const
		{
			WGINE_CORE_TRACE("{0}VertexBuffer of size {1}:", std::string(indent, '\t'), DEBUGcount);
			for (int i = 0; i < DEBUGcount; i++)
				WGINE_CORE_TRACE("{0}{1}: {2}", std::string(indent + 1, '\t'), i, std::string(DEBUGdata[i]));
		}

		virtual void SetData(/*const*/ void *data, uint32_t typeSize, uint32_t count, uint32_t offset = 0) override;

		inline virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }
		inline virtual const BufferLayout &GetBufferLayout() const override { return m_Layout; }

	private:
		GLuint m_Ptr;
		BufferLayout m_Layout;

		std::vector <Vertex> DEBUGdata;
		uint32_t DEBUGcount;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count);
		OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void PrintDebug(int indent) const override
		{
			WGINE_CORE_TRACE("{0}IndexBuffer of size {1}:", std::string(indent, '\t'), DEBUGcount);
			for (int i = 0; i < DEBUGcount; i++)
				WGINE_CORE_TRACE("{0}{1}: {2}", std::string(indent + 1, '\t'), i, DEBUGdata[i]);
		}

		virtual void SetData(/*const*/ uint32_t *data, uint32_t count, uint32_t offset = 0) override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		GLuint m_Ptr;
		uint32_t m_Count;

		std::vector<uint32_t> DEBUGdata;
		uint32_t DEBUGcount;
	};
}
