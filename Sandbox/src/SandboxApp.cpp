#include "Wgine.h"

using namespace Wgine;

class ExampleLayer : public Wgine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		m_Camera = PerspectiveCamera(Transform(), 45.f, 1600, 900, 0.1f, 100000.f);
		m_Camera.SetPosition({ 0.f, 0.f, 5.f });
		m_Camera.SetRotation({ 0.f, 90.f, 0.f });
		auto forward = m_Camera.GetTransform().GetForwardVector();
		auto right = m_Camera.GetTransform().GetRightVector();
		auto up = m_Camera.GetTransform().GetUpVector();
		//WGINE_CORE_INFO("Forward: {0}, Right: {1}, Up: {2}",
		//	m_Camera.GetTransform().GetForwardVector(),
		//	m_Camera.GetTransform().GetRightVector(),
		//	m_Camera.GetTransform().GetUpVector());

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

	void OnUpdate() override
	{
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
		RenderCommand::Clear();

		auto deltaSeconds = Time::GetDeltaSeconds();
		WGINE_CORE_TRACE("Delta time: {0} s, FPS: {1}", deltaSeconds, 1.f / deltaSeconds);
		auto speed = 60.f;
		if (Input::IsKeyPressed(WGINE_KEY_W))
		{
			m_Camera.SetPosition(m_Camera.GetTransform().Position + m_Camera.GetTransform().GetForwardVector() * speed * deltaSeconds);
		}
		if (Input::IsKeyPressed(WGINE_KEY_S))
		{
			m_Camera.SetPosition(m_Camera.GetTransform().Position + m_Camera.GetTransform().GetForwardVector() * (-speed * deltaSeconds));
		}
		if (Input::IsKeyPressed(WGINE_KEY_D))
		{
			m_Camera.SetPosition(m_Camera.GetTransform().Position + m_Camera.GetTransform().GetRightVector() * (speed * deltaSeconds));
		}
		if (Input::IsKeyPressed(WGINE_KEY_A))
		{
			m_Camera.SetPosition(m_Camera.GetTransform().Position + m_Camera.GetTransform().GetRightVector() * (-speed * deltaSeconds));
		}

		Renderer::BeginScene(m_Camera); {

			Renderer::Submit(m_SquareShader, m_SquareVertexArray);
			Renderer::Submit(m_Shader, m_VertexArray);

		} Renderer::EndScene();
	}

	void OnEvent(Wgine::Event &event) override
	{
		;
	}

private:
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<VertexArray> m_VertexArray;

	std::shared_ptr<Shader> m_SquareShader;
	std::shared_ptr<VertexArray> m_SquareVertexArray;

	Camera m_Camera;
};

class Sandbox : public Wgine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Wgine::Application* Wgine::CreateApplication()
{
	return new Sandbox();
}
