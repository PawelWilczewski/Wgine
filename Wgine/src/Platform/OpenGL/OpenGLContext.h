#pragma once

#include "Wgine/Renderer/RenderingContext.h"

struct GLFWwindow;

namespace Wgine
{
	class OpenGLContext : public RenderingContext
	{
	public:
		OpenGLContext(GLFWwindow *window);

		virtual void SwapBuffers() override;

	private:
		GLFWwindow *m_Window;
	};
}
