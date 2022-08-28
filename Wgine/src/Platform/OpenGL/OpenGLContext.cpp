#include "WginePCH.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace Wgine
{
	OpenGLContext::OpenGLContext(GLFWwindow *window)
		: m_Window(window)
	{
		WGINE_CORE_ASSERT(window, "Window handle is invalid!");

		glfwMakeContextCurrent(window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		WGINE_CORE_ASSERT(status, "Failed to initialize glad!");

		WGINE_CORE_INFO("OpenGL Renderer:");
		WGINE_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
		WGINE_CORE_INFO("    Renderer: {0}", glGetString(GL_RENDERER));
		WGINE_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));

		// We're using left-handed coordinate system
		glFrontFace(GL_CW);
		glDepthFunc(GL_GEQUAL);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}
