#include "WginePCH.h"
#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Wgine/Log.h"

#include <GLFW/glfw3.h>

namespace Wgine {
	Wgine::Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Wgine::Application::~Application()
	{
	}

	void Wgine::Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();
		}
	}
}
