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


		// opengl triangle data
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-.5f, -.5f, 0.f,
			0.5f, -0.5f, 0.f,
			0.f, 0.5f, 0.f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = {
			0, 1, 2
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Wgine::Application::~Application()
	{
	}

	void Wgine::Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.15f, 0.15f, 0.15f, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

	void Application::OnEvent(Event &e)
	{
		EventDispatcher d(e);
		d.Dispatch<WindowCloseEvent>(WGINE_BIND_EVENT_FN(Application::OnWindowClosed));

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

	bool Application::OnWindowClosed(WindowCloseEvent &e)
	{
		m_Running = false;

		return true;
	}
}
