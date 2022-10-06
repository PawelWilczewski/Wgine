#include "WginePCH.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Wgine
{
	Ref<Shader> Shader::Create(const std::string &sourceFilePath)
	{
		SWITCH_RENDERAPI_MAKEREF(Shader, sourceFilePath);
	}
	Ref<Shader> Shader::Create(const std::string &displayedPath, const std::string &vertexSource, const std::string &fragmentSource)
	{
		SWITCH_RENDERAPI_MAKEREF(Shader, displayedPath, vertexSource, fragmentSource);
	}
}
