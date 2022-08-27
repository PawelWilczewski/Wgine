#include "WginePCH.h"
#include "Application.h"

#include "Input.h"
#include "Renderer/Buffer.h"

#include "Wgine/Renderer/Renderer.h"
#include "Wgine/Renderer/RenderCommand.h"

#include "Wgine/Camera.h"

namespace Wgine {

	Application *Application::s_Instance = nullptr;

	Wgine::Application::Application()
		: m_Window(Window::Create()), m_Camera(PerspectiveCamera(Transform(), 45.f, m_Window->GetWidth(), m_Window->GetHeight(), 0.1f, 100000.f))
	{
		WGINE_CORE_ASSERT(s_Instance == nullptr, "Attempting to create multiple applications!");
		s_Instance = this;

		m_Window->SetEventCallback(WGINE_BIND_EVENT_FN(Application::OnEvent));

		m_Camera.SetPosition({ 0.f, 0.f, 5.f });
		m_Camera.SetRotation({ 0.f, 0.f, 30.f });

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// triangle data
		m_VertexArray.reset(VertexArray::Create());

		// triangle vertex buffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.4f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.2f, 0.5f, 0.9f, 1.0f,
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// triangle index buffer
		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// triangle shaders
		std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);

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

		// square data
		m_SquareVertexArray.reset(VertexArray::Create());

		// square vertex buffer
		float verticesSquare[3 * 4] = {
			-0.6f,  0.6f, 0.0f,
			-0.6f, -0.6f, 0.0f,
			 0.6f, -0.6f, 0.0f,
			 0.6f,  0.6f, 0.0f,
		};
		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
		squareVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		// square indices
		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		// square shaders
		std::string squareVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 Color;
			
			in vec3 v_Position;

			void main()
			{
				Color = vec4(0.5, 0.8, 0.2, 1.0);
			}
		)";

		m_SquareShader.reset(Shader::Create(squareVertexSource, squareFragmentSource));
	}

	Wgine::Application::~Application()
	{
	}

	void Wgine::Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera); {

				Renderer::Submit(m_SquareShader, m_SquareVertexArray);
				Renderer::Submit(m_Shader, m_VertexArray);

			} Renderer::EndScene();

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
