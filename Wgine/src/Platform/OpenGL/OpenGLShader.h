#pragma once

#include "Wgine/Renderer/Shader.h"
#include <glad/glad.h>
#include <unordered_map>
#include <string>

namespace Wgine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string &sourceFilePath);
		OpenGLShader(const std::string &displayedPath, const std::string &vertexSource, const std::string &fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformInt(const std::string &name, const int &val) override;
		virtual void UploadUniformFloat(const std::string &name, const float &val) override;
		virtual void UploadUniformFloat2(const std::string &name, const glm::vec2 &val) override;
		virtual void UploadUniformFloat3(const std::string &name, const glm::vec3 &val) override;
		virtual void UploadUniformFloat4(const std::string &name, const glm::vec4 &val) override;
		virtual void UploadUniformMat3(const std::string &name, const glm::mat3 &val) override;
		virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &val) override;
		virtual void UploadUniformIntArray(const std::string &name, const int *val, uint32_t count) override;

		virtual void SetupStorageBuffer(const std::string &name, int slot, uint32_t ssbo) override;

	private:
		void Compile(const std::unordered_map<GLenum, std::string> &source);
		std::unordered_map<GLenum, std::string> ExtractShadersSource(const std::string &fileSource);

		GLint GetUniformLocation(const std::string &name);

	private:
		uint32_t m_RendererID;
		std::unordered_map<std::string, GLint> m_UniformLocations;
	};
}
