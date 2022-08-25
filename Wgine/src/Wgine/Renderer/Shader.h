#pragma once

#include <string>

namespace Wgine
{
	class Shader
	{
	public:
		Shader(const std::string &vertexSource, const std::string &fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

	private:
		uint32_t m_RendererID;
	};
}
