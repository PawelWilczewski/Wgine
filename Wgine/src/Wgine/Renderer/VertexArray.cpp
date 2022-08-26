#include "WginePCH.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Wgine
{
	VertexArray *VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
			case Renderer::API::OpenGL:	return new OpenGLVertexArray();
		}

		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
