#include "WginePCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Wgine
{
	// VertexBuffer implementation -----------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t typeSize, uint32_t count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, typeSize * count, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t typeSize, uint32_t count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, typeSize * count, nullptr, GL_DYNAMIC_DRAW);
		SetData(vertices, typeSize, count);
		//glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		//glBufferData(GL_ARRAY_BUFFER, typeSize * count, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_Ptr);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(/*const*/ void *data, uint32_t typeSize, uint32_t count, uint32_t offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferSubData(GL_ARRAY_BUFFER, typeSize * offset, typeSize * count, data);

		DEBUGdata.insert(DEBUGdata.end(), (Vertex *)data, (Vertex *)data + count);
		DEBUGcount = offset + count;
	}

	// IndexBuffer implementation ------------------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 10000, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, nullptr, GL_DYNAMIC_DRAW);
		SetData(indices, count);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_Ptr);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::SetData(/*const*/ uint32_t *data, uint32_t count, uint32_t offset)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * offset, sizeof(uint32_t) * count, data);

		DEBUGdata.insert(DEBUGdata.end(), data, data + count);
		DEBUGcount = offset + count;
	}
}
