#include "WginePCH.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Wgine
{
	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case Renderer::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case Renderer::API::OpenGL:	return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
