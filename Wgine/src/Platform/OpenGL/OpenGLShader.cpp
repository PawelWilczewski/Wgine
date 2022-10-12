#include "WginePCH.h"
#include "OpenGLShader.h"

#include "Wgine/Core/FileUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <regex>
#include <filesystem>

namespace Wgine
{
	static GLenum StringToShaderType(const std::string &str)
	{
		if (str == "vertex")
			return GL_VERTEX_SHADER;
		else if (str == "fragment" || str == "pixel")
			return GL_FRAGMENT_SHADER;
		
		WGINE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string &sourceFilePath)
	{
		Compile(ExtractShadersSource(FileUtils::ReadFile(sourceFilePath)));

		m_Path = sourceFilePath;
	}

	OpenGLShader::OpenGLShader(const std::string &displayedPath, const std::string &vertexSource, const std::string &fragmentSource)
	{
		std::unordered_map<GLenum, std::string> data;
		data[GL_VERTEX_SHADER] = vertexSource;
		data[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(data);

		m_Path = displayedPath;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> &data)
	{
		// Get a program object.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		std::vector<GLuint> shaders;
		shaders.reserve(data.size());
		for (const auto &s : data)
		{
			auto &[type, source] = s;

			// Create an empty vertex Shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex Shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar *s = (const GLchar *)source.c_str();
			glShaderSource(shader, 1, &s, 0);

			// Compile the vertex Shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the Shader anymore.
				glDeleteShader(shader);

				WGINE_CORE_ERROR("Error during vertex shader compilation: {0}", infoLog.data());
				WGINE_CORE_ASSERT(false, "Vertex shader compilation error!");
				return;
			}

			shaders.push_back(shader);
			glAttachShader(program, shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (const auto &shader : shaders)
				glDeleteShader(shader);

			WGINE_CORE_ERROR("Error during shader linking: {0}", infoLog.data());
			WGINE_CORE_ASSERT(false, "Shader linking error!");
			return;
		}

		// Always detach shaders after a successful link.
		for (const auto &shader : shaders)
			glDetachShader(program, shader);
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ExtractShadersSource(const std::string &fileSource)
	{
		std::unordered_map<GLenum, std::string> result;
		const char *token = "#type";
		auto start = fileSource.find(token);
		while (fileSource.length() > start)
		{
			int i;
			for (i = start + 5; i < fileSource.length(); i++) // skip through all the whitespaces
				if (fileSource[i] != ' ' && fileSource[i] != '\t' && fileSource[i] != '\v')
					break;

			// determine the type
			auto endline = fileSource.find_first_of("\n\r", i);
			WGINE_CORE_ASSERT(endline != std::string::npos, "Shader parsing error! {0}", fileSource);
			std::string type = fileSource.substr(i, endline - i);
			i += type.length();

			// finished? the loop is gonna terminate if end is npos
			auto end = fileSource.find(token, start + strlen(token));
			result[StringToShaderType(type)] = fileSource.substr(i, end - i);
			start = end;
		}

		return result;
	}

	GLint OpenGLShader::GetUniformLocation(const std::string &name)
	{
		if (m_UniformLocations.find(name) != m_UniformLocations.end())
			return m_UniformLocations[name];

		GLint result = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocations[name] = result;
		return result;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string &name, const int &val)
	{
		glUniform1i(GetUniformLocation(name), val);
	}

	void OpenGLShader::UploadUniformFloat(const std::string &name, const float &val)
	{
		glUniform1f(GetUniformLocation(name), val);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &val)
	{
		glUniform2f(GetUniformLocation(name), val.x, val.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &val)
	{
		glUniform3f(GetUniformLocation(name), val.x, val.y, val.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &val)
	{
		glUniform4f(GetUniformLocation(name), val.x, val.y, val.z, val.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &val)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
	}

	void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &val)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
	}

	void OpenGLShader::UploadUniformIntArray(const std::string &name, int *val, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, val);
	}

	void OpenGLShader::SetupStorageBuffer(const std::string &name, int slot, uint32_t ssbo)
	{
		GLuint block = glGetProgramResourceIndex(m_RendererID, GL_SHADER_STORAGE_BLOCK, name.c_str());
		glShaderStorageBlockBinding(m_RendererID, block, slot);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, ssbo);
	}
}
