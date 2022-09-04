#include "WginePCH.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Wgine
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void *data)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case Renderer::API::OpenGL:	return MakeRef<OpenGLTexture2D>(width, height, data);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string &path)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::None:	WGINE_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;
		case Renderer::API::OpenGL:	return MakeRef<OpenGLTexture2D>(path);
		}
		WGINE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
