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
		Compile(ExtractShadersSource(sourceFilePath, FileUtils::ReadFile(sourceFilePath)));

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

	// first lambda argument is the text after the token between the double quotation marks "
	// second argument is the following text until the next token of the same arg occurs
	// returns the substring before the first token
	static std::string ParseToken(const std::string &token, const std::string &source, std::function<void(const std::string &, const std::string &)> parseCallback)
	{
		auto start = source.find(token);

		std::string before;
		if (start == std::string::npos)
			before = source;
		else
			before = source.substr(0, start);

		while (source.length() > start)
		{
			// determine the arg
			auto firstQuotation = source.find('"', start);
			auto secondQuotation = source.find('"', firstQuotation + 1);

			WGINE_CORE_ASSERT(firstQuotation != std::string::npos && secondQuotation != std::string::npos, "Shader parsing error (probably missing \")! {0}", source);
			std::string arg = source.substr(firstQuotation + 1, secondQuotation - firstQuotation - 1);

			// finished? the loop is gonna terminate if end is npos
			auto end = source.find(token, start + token.length());
			parseCallback(arg, source.substr(secondQuotation + 1, end - secondQuotation - 1));
			start = end;
		}

		return before;
	}

	// can be nested includes (when #include appears in an included file)
	// the limitation is you can't yet include the same file twice in one file, regardless of which shader type the include directive is in
	// also, DON'T type #include in comments
	static std::string ResolveIncludes(const std::string &filePath, const std::string &fileSource)
	{
		// TODO: important: since we have multiple shaders in one file, we need to append the shader type to the file
		//  so we can include the same file multiple times as long as it is in different shader types
		std::vector <std::string> filesIncluded = { filePath };
		std::string result = fileSource;
		std::string toParse = fileSource;

		while (toParse.find("#include") != std::string::npos)
		{
			result = "";
			std::string before = ParseToken("#include", toParse, [&](const std::string &arg, const std::string &next) {
				auto p = std::filesystem::path(filePath);
				auto argFile = p.parent_path(); argFile += "/" + arg;
				auto argFileStr = argFile.generic_string();

				// skip if file already included
				if (std::find(filesIncluded.begin(), filesIncluded.end(), argFileStr) != filesIncluded.end())
				{
					result += next;
					return;
				}
				else
					filesIncluded.push_back(argFileStr);

				result += FileUtils::ReadFile(argFileStr) + next;
				//finalSrc += "\r\n" + next; // just to be safe, ensure the newline as well
			});
			result = before + result;
			toParse = result;
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ExtractShadersSource(const std::string &filePath, const std::string &fileSource)
	{
		std::unordered_map<GLenum, std::string> result;

		auto src = ResolveIncludes(filePath, fileSource);
		ParseToken("#type", src, [&](const std::string &arg, const std::string &next) {
			result[StringToShaderType(arg)] = next;
		});

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

	void OpenGLShader::UploadUniformIntArray(const std::string &name, const int *val, uint32_t count)
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
