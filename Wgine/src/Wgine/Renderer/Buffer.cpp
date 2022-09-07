#include "WginePCH.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Wgine
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		SWITCH_RENDERAPI_MAKEREF(VertexBuffer, size);
	}
	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t size)
	{
		SWITCH_RENDERAPI_MAKEREF(VertexBuffer, vertices, size);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
	{
		SWITCH_RENDERAPI_MAKEREF(IndexBuffer, indices, count);
	}
}
