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
			case RendererAPI::None:		WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
			default:					WGINE_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
		return nullptr;
	}

	IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, size);
			default:					WGINE_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
		return nullptr;
	}
}
