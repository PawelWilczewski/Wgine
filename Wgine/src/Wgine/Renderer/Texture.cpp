#include "WginePCH.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Wgine
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, void *data)
	{
		SWITCH_RENDERAPI_MAKEREF(Texture2D, width, height, data);
	}

	Ref<Texture2D> Texture2D::Create(const std::string &path)
	{
		SWITCH_RENDERAPI_MAKEREF(Texture2D, path);
	}
}
