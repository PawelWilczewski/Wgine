#pragma once

#include "Shader.h"

namespace Wgine
{
	class ShaderLibrary
	{
	public:		
		// Loads/gets the shader at specified file path. If not loaded, it will proceed with that; otherwise just return
		static Ref<Shader> Get(const std::string &filePath);
	};
}
