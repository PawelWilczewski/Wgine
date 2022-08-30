#include "WginePCH.h"
#include "OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Wgine
{
	OpenGLShader::OpenGLShader(const std::string &vertexSource, const std::string &fragmentSource)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *source = (const GLchar *)vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			WGINE_CORE_ERROR("Error during vertex shader compilation: {0}", infoLog.data());
			WGINE_CORE_ASSERT(false, "Vertex shader compilation error!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar *)fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			WGINE_CORE_ERROR("Error during fragment shader compilation: {0}", infoLog.data());
			WGINE_CORE_ASSERT(false, "Fragment shader compilation error!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

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
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			WGINE_CORE_ERROR("Error during shader linking: {0}", infoLog.data());
			WGINE_CORE_ASSERT(false, "Shader linking error!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
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
}
