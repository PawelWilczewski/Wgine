#include "WginePCH.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Wgine
{
	VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case Renderer::API::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case Renderer::API::OpenGL:	return new OpenGLIndexBuffer(indices, size);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
