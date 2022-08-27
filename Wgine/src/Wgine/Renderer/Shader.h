#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Wgine
{
	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &mat) = 0;

		static Shader *Create(const std::string &vertexSource, const std::string &fragmentSource);
	};
}
