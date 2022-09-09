#include "WginePCH.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Wgine
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t typeSize, uint32_t count)
	{
		SWITCH_RENDERAPI_MAKEREF(VertexBuffer, typeSize, count);
	}
	Ref<VertexBuffer> VertexBuffer::Create(float *vertices, uint32_t typeSize, uint32_t count)
	{
		SWITCH_RENDERAPI_MAKEREF(VertexBuffer, vertices, typeSize, count);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count)
	{
		SWITCH_RENDERAPI_MAKEREF(IndexBuffer, count);
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t *indices, uint32_t count)
	{
		SWITCH_RENDERAPI_MAKEREF(IndexBuffer, indices, count);
	}
}
