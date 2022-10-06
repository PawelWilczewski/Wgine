#include "WginePCH.h"
#include "ShaderLibrary.h"
#include <filesystem>

namespace Wgine
{
	static std::unordered_map<std::string, Ref<Shader>> s_Shaders;

	//static void Add(const std::string &filePath, Ref<Shader> shader)
	//{
	//	if (Exists(shader->GetName()))
	//		WGINE_CORE_WARN("Shader library: readding shader '{0}'", filePath);
	//	s_Shaders[filePath] = shader;
	//}
	//void ShaderLibrary::Add(const Ref<Shader> &shader)
	//{
	//	if (Exists(shader->GetName()))
	//		WGINE_CORE_WARN("Shader library: overriding shader with name '{0}'", shader->GetName());
	//	s_Shaders[shader->GetName()] = shader;
	//}

	//void ShaderLibrary::Add(const std::string &shaderName, const Ref<Shader> &shader)
	//{
	//	if (Exists(shaderName))
	//		WGINE_CORE_WARN("Shader library: overriding shader with name '{0}'", shaderName);
	//	s_Shaders[shaderName] = shader;
	//}

	//Ref<Shader> ShaderLibrary::Load(const std::string &filePath)
	//{
	//	auto shader = Shader::Create(filePath);
	//	Add(filePath, shader);
	//	return shader;
	//}

	//Ref<Shader> ShaderLibrary::Load(const std::string &shaderName, const std::string &filePath)
	//{
	//	auto shader = Shader::Create(filePath);
	//	Add(shaderName, shader);
	//	return shader;
	//}

	//Ref<Shader> ShaderLibrary::Get(const std::string &shaderName)
	//{
	//	WGINE_CORE_ASSERT(Exists(shaderName), "Shader library: shader doesn't exist '{0}'", shaderName);
	//	return s_Shaders[shaderName];
	//}

	Ref<Shader> ShaderLibrary::Get(const std::string &filePath)
	{
		if (Exists(filePath))
			return s_Shaders[filePath];

		auto shader = Shader::Create(filePath);
		s_Shaders[filePath] = shader;
		return shader;
	}

	//bool ShaderLibrary::Exists(const std::string &shaderName)
	//{
	//	return s_Shaders.find(shaderName) != s_Shaders.end();
	//}

	//static bool Exists(const std::string &shaderName);
	bool ShaderLibrary::Exists(const std::string &filePath)
	{
		return s_Shaders.find(filePath) != s_Shaders.end();
	}
}
