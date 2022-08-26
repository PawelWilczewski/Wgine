#include "WginePCH.h"
#include "RendererCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Wgine
{
	RendererAPI *RendererCommand::s_RendererAPI = new OpenGLRendererAPI();
}
