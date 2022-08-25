#include "WginePCH.h"
#include "Application.h"

#include "Input.h"
#include "Renderer/Buffer.h"

#include <glad/glad.h>

namespace Wgine {

	Application *Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType &type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		WGINE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.5f, 0.9f, 1.0f,
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
			};
			m_VertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		for (const auto &element : m_VertexBuffer->GetBufferLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				m_VertexBuffer->GetBufferLayout().GetStride(),
				(const void *)element.Offset);
			index++;
		}


		unsigned int indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);

				v_Color = a_Color;
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				// Color = vec4(0.5, 0.8, 0.2, 1.0);
				Color = v_Color;
			}
		)";

		m_Shader.reset(Shader::Create(vertexSource, fragmentSource));
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

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
