#include "WginePCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Wgine
{
	// VertexBuffer implementation -----------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
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

	void OpenGLVertexBuffer::SetData(const void *data, uint32_t size, uint32_t offsetBytes)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferSubData(GL_ARRAY_BUFFER, offsetBytes, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, nullptr, GL_DYNAMIC_DRAW);
	}

	// IndexBuffer implementation ------------------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
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

	void OpenGLIndexBuffer::SetData(const void *data, uint32_t count, uint32_t offsetCount)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * offsetCount, sizeof(uint32_t) * count, data);
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(const void *data, uint32_t size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_COPY);
	}

	OpenGLStorageBuffer::~OpenGLStorageBuffer()
	{
		glDeleteBuffers(1, &m_Ptr);
	}

	void OpenGLStorageBuffer::Bind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);

		
	}

	void OpenGLStorageBuffer::Unbind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLStorageBuffer::SetData(const void *data, uint32_t size, uint32_t offsetBytes)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetBytes, size, data);

		//GLuint block_index = glGetProgramResourceIndex(m_Ptr, GL_SHADER_STORAGE_BLOCK, "ss_Materials");
		//GLuint ssbo_binding_point_index = 0;
		//glShaderStorageBlockBinding(m_Ptr, block_index, ssbo_binding_point_index);
	}
}
