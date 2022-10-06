#pragma once

#include "Shader.h"

namespace Wgine
{
	class ShaderLibrary
	{
	public:
		//static void Add(const Ref<Shader> &shader);
		//static void Add(const std::string &shaderName, const Ref<Shader> &shader);

		//static Ref<Shader> Load(const std::string &filePath);
		//static Ref<Shader> Load(const std::string &shaderName, const std::string &filePath);
		// 
		//static Ref<Shader> Get(const std::string &shaderName);
		
		// Loads/gets the shader at specified file path. If not loaded, it will proceed with that; otherwise just return
		static Ref<Shader> Get(const std::string &filePath);

	private:
		//static bool Exists(const std::string &shaderName);
		static bool Exists(const std::string &filePath);
	};
}
