#include "WginePCH.h"
#include "ShaderLibrary.h"
#include <filesystem>

namespace Wgine
{
	static std::unordered_map<std::string, Ref<Shader>> s_Shaders;

	Ref<Shader> ShaderLibrary::Get(const std::string &filePath)
	{
		if (s_Shaders.find(filePath) != s_Shaders.end())
			return s_Shaders[filePath];

		auto shader = Shader::Create(filePath);
		s_Shaders[filePath] = shader;
		return shader;
	}
}
