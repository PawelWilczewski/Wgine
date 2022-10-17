#include "WginePCH.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Wgine
{
	// VertexBuffer implementation -----------------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(void *vertices, uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
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

		// automatically resize to twice the size if exceeded limit
		auto fullSize = offsetBytes + size;
		if (fullSize > m_Size)
		{
			m_Size = fullSize * 2;
			glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);
		}

		glBufferSubData(GL_ARRAY_BUFFER, offsetBytes, size, data);
	}

	void OpenGLVertexBuffer::Resize(uint32_t newSize)
	{
		if (newSize <= m_Size) return;

		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
		m_Size = newSize;
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
		glBindBuffer(GL_ARRAY_BUFFER, m_Ptr);

		// automatically resize to twice the size if exceeded limit
		auto fullCount = offsetCount + count;
		if (fullCount > m_Count)
		{
			m_Count = fullCount * 2;
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Count, nullptr, GL_DYNAMIC_DRAW);
		}

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * offsetCount, sizeof(uint32_t) * count, data);
	}

	void OpenGLIndexBuffer::Resize(uint32_t newCount)
	{
		if (newCount <= m_Count) return;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ptr);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * newCount, nullptr, GL_DYNAMIC_DRAW);
		m_Count = newCount;
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(uint32_t size)
		: m_Size(size)
	{
		glCreateBuffers(1, &m_Ptr);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_COPY);
	}

	OpenGLStorageBuffer::OpenGLStorageBuffer(const void *data, uint32_t size)
		: m_Size(size)
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

		// automatically resize to twice the size if exceeded limit
		auto fullSize = offsetBytes + size;
		if (fullSize > m_Size)
		{
			m_Size = fullSize * 2;
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, nullptr, GL_DYNAMIC_COPY);
		}

		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetBytes, size, data);
	}

	void OpenGLStorageBuffer::Resize(uint32_t newSize)
	{
		if (newSize <= m_Size) return;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Ptr);
		glBufferData(GL_SHADER_STORAGE_BUFFER, newSize, nullptr, GL_DYNAMIC_COPY);
		m_Size = newSize;
	}
}
