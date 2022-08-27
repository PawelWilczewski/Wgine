#pragma once

#include "Wgine/Renderer/Shader.h"

namespace Wgine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource);
		virtual ~OpenGLShader();

		virtual void Bind();
		virtual void Unbind();
		virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &mat);

	private:
		uint32_t m_RendererID;
	};
}
