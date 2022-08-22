#include "WginePCH.h"
#include "Application.h"

#include "Wgine/Log.h"

#include <GLFW/glfw3.h>

namespace Wgine {

#define BIND_EVENT(x) (std::bind(&Application::x, this, std::placeholders::_1))

	Wgine::Application::Application()
	{
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT(OnEvent));
	}

	Wgine::Application::~Application()
	{
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(BIND_EVENT(OnWindowClosed));

		WGINE_CORE_TRACE("Event happened: {0}", e);
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

	bool Application::OnWindowClosed(WindowCloseEvent &e)
	{
		m_Running = false;

		return true;
	}
}
