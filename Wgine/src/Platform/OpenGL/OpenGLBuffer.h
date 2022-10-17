#pragma once

#include "Wgine/Renderer/Buffer.h"

namespace Wgine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(void *vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32_t size, uint32_t offsetBytes = 0) override;
		virtual void Resize(uint32_t newSize) override;
		virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }

		virtual uint32_t GetSize() const override { return m_Size; }
		virtual const BufferLayout &GetBufferLayout() const override { return m_Layout; }

	private:
		uint32_t m_Ptr;
		uint32_t m_Size;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t count);
		OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32_t count, uint32_t offsetCount = 0) override;
		virtual void Resize(uint32_t newCount) override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_Ptr;
		uint32_t m_Count;
	};

	class OpenGLStorageBuffer : public StorageBuffer
	{
	public:
		OpenGLStorageBuffer(uint32_t size);
		OpenGLStorageBuffer(const void *data, uint32_t size);
		virtual ~OpenGLStorageBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32_t size, uint32_t offsetBytes = 0) override;
		virtual void Resize(uint32_t newSize) override;

		virtual uint32_t GetPtr() const override { return m_Ptr; }
		virtual uint32_t GetSize() const { return m_Size; }

	private:
		uint32_t m_Ptr;
		uint32_t m_Size;
	};
}
