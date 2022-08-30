#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Wgine
{
	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// TODO: will be removed later cos it's an opengl-only thing
		virtual void UploadUniformInt(const std::string &name, const int &val) = 0;
		virtual void UploadUniformFloat(const std::string &name, const float &val) = 0;
		virtual void UploadUniformFloat2(const std::string &name, const glm::vec2 &val) = 0;
		virtual void UploadUniformFloat3(const std::string &name, const glm::vec3 &val) = 0;
		virtual void UploadUniformFloat4(const std::string &name, const glm::vec4 &val) = 0;
		virtual void UploadUniformMat3(const std::string &name, const glm::mat3 &val) = 0;
		virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &val) = 0;

		static Shader *Create(const std::string &vertexSource, const std::string &fragmentSource);
	};
}
