#include "WginePCH.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Wgine
{
	Ref<VertexArray> VertexArray::Create()
	{
		SWITCH_RENDERAPI_MAKEREF(VertexArray);
	}
}
