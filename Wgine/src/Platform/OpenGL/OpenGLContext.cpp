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
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}
