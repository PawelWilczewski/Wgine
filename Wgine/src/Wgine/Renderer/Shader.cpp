#include "WginePCH.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Wgine
{
	Shader *Shader::Create(const std::string &sourceFilePath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case Renderer::API::OpenGL:	return new OpenGLShader(sourceFilePath);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Shader *Shader::Create(const std::string &vertexSource, const std::string &fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case Renderer::API::OpenGL:	return new OpenGLShader(vertexSource, fragmentSource);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
