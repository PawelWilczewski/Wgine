#include "WginePCH.h"
#include "Application.h"

#include "Input.h"

#include <glad/glad.h>

namespace Wgine {

	Application *Application::s_Instance = nullptr;

	Wgine::Application::Application()
	{
		WGINE_CORE_ASSERT(s_Instance == nullptr, "Attempting to create multiple applications!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(WGINE_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Wgine::Application::~Application()
	{
	}

	void Application::OnEvent(Event &e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(WGINE_BIND_EVENT_FN(Application::OnWindowClosed));

		WGINE_CORE_TRACE("Event happened: {0}", e);

		for (auto i = m_LayerStack.end(); i != m_LayerStack.begin(); )
		{
			(*--i)->OnEvent(e);
			if (e.WasHandled())
				break;
		}
	}

	void Application::PushLayer(Layer *layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer *overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Wgine::Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto layer : m_LayerStack)
				layer->OnUpdate();

			// ImGui rendering
			m_ImGuiLayer->Begin();
			for (Layer *layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent &e)
	{
		m_Running = false;

		return true;
	}
}
