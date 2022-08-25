#pragma once

#include <string>

namespace Wgine
{
	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Shader *Create(const std::string &vertexSource, const std::string &fragmentSource);
	};
}
