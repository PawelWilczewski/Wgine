#include "WginePCH.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Wgine
{
	Shader *Shader::Create(const std::string &vertexSource, const std::string &fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLShader(vertexSource, fragmentSource);
		default:					WGINE_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
		return nullptr;
	}
}
