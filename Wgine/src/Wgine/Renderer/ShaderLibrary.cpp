#include "WginePCH.h"
#include "ShaderLibrary.h"
#include <filesystem>

namespace Wgine
{
	void ShaderLibrary::Add(const Ref<Shader> &shader)
	{
		if (Exists(shader->GetName()))
			WGINE_CORE_WARN("Shader library: overriding shader with name '{0}'", shader->GetName());
		m_Shaders[shader->GetName()] = shader;
	}

	void ShaderLibrary::Add(const std::string &shaderName, const Ref<Shader> &shader)
	{
		if (Exists(shaderName))
			WGINE_CORE_WARN("Shader library: overriding shader with name '{0}'", shaderName);
		m_Shaders[shaderName] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string &filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string &shaderName, const std::string &filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shaderName, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string &shaderName)
	{
		WGINE_CORE_ASSERT(Exists(shaderName), "Shader library: shader doesn't exist '{0}'", shaderName);
		return m_Shaders[shaderName];
	}
}
