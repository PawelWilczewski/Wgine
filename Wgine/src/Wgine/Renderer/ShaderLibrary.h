#pragma once

#include "Shader.h"

namespace Wgine
{
	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader> &shader);
		void Add(const std::string &shaderName, const Ref<Shader> &shader);
		Ref<Shader> Load(const std::string &filePath);
		Ref<Shader> Load(const std::string &shaderName, const std::string &filePath);
		Ref<Shader> Get(const std::string &shaderName);

		bool Exists(const std::string &shaderName) const { return m_Shaders.find(shaderName) != m_Shaders.end(); };

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
