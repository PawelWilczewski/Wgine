#include "Wgine.h"

using namespace Wgine;

class ExampleLayer : public Wgine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		m_Camera = PerspectiveCamera(Transform(), 45.f, 1600, 900, 0.1f, 100000.f);
		//m_Camera = OrthographicCamera(Transform(), -1.6f, 1.6f, -0.9f, 0.9f);
		m_Camera.SetLocation({ 0.f, 0.f, 0.f });
		m_Triangle = std::make_unique<SceneEntity>();
		// triangle data
		{
			m_Triangle->MeshData.reset(VertexArray::Create());
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
			m_Triangle->MeshData->AddVertexBuffer(vertexBuffer);

			// triangle index buffer
			unsigned int indices[3] = { 0, 1, 2 };
			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_Triangle->MeshData->SetIndexBuffer(indexBuffer);

			// triangle shaders
			std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);

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

			m_Triangle->ShaderData.reset(Shader::Create(vertexSource, fragmentSource));
		}

		m_Square = std::make_unique<SceneEntity>();
		m_Square->SetRotation({ 30.f, 0.f, 0.f });
		// square data
		{
			m_Square->MeshData.reset(VertexArray::Create());

			// square vertex buffer
			float verticesSquare[3 * 8] = {
				-1.0f,  1.0f, -0.5f,
				-1.0f, -1.0f, -0.5f,
				 1.0f, -1.0f, -0.5f,
				 1.0f,  1.0f, -0.5f,
				 1.0f, -1.0f,  0.2f,
				 1.0f, -1.0f,  0.8f,
				 1.0f,  1.0f,  0.8f,
				 1.0f,  1.0f,  0.2f,
			};
			std::shared_ptr<VertexBuffer> squareVertexBuffer;
			squareVertexBuffer.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
			squareVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				});
			m_Square->MeshData->AddVertexBuffer(squareVertexBuffer);

			// square indices
			unsigned int squareIndices[12] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
			std::shared_ptr<IndexBuffer> squareIndexBuffer;
			squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
			m_Square->MeshData->SetIndexBuffer(squareIndexBuffer);

			// square shaders
			std::string squareVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

			m_Square->ShaderData.reset(Shader::Create(squareVertexSource, squareFragmentSource));
		}

		m_Axis = std::make_unique<SceneEntity>();
		m_AxisCamera = std::make_unique<SceneEntity>();
		//m_Axis->SetRotation({ 0.f, 30.f, 0.f });
		// axis data
		{
			m_Axis->MeshData.reset(VertexArray::Create());
			m_AxisCamera->MeshData.reset(VertexArray::Create());
			// triangle vertex buffer
			float vertices[10 * 7] = {
				 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				 0.0f,  0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				 1.0f,  0.1f, 0.0f, 0.9f, 0.2f, 0.3f, 1.0f, // red
				 1.0f,  0.0f, 0.0f, 0.9f, 0.2f, 0.3f, 1.0f, // red

				 0.0f,  1.0f, 0.0f, 0.5f, 0.8f, 0.2f, 1.0f, // green
				 0.0f,  1.0f, 0.1f, 0.5f, 0.8f, 0.2f, 1.0f, // green

				 0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				 0.0f,  0.0f, 1.0f, 0.2f, 0.5f, 0.9f, 1.0f, // blue
				 0.1f,  0.0f, 1.0f, 0.2f, 0.5f, 0.9f, 1.0f, // blue

				 0.0f,  0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 1.0f,
			};
			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				});
			m_Axis->MeshData->AddVertexBuffer(vertexBuffer);
			m_AxisCamera->MeshData->AddVertexBuffer(vertexBuffer);

			// triangle index buffer
			unsigned int indices[18] = { 0, 1, 2, 0, 2, 3,
										0, 4, 5, 5, 9, 0,
										0, 6, 7, 7, 8, 6 };
			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
			m_Axis->MeshData->SetIndexBuffer(indexBuffer);
			m_AxisCamera->MeshData->SetIndexBuffer(indexBuffer);

			// triangle shaders
			std::string vertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);

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

			m_Axis->ShaderData.reset(Shader::Create(vertexSource, fragmentSource));
			m_AxisCamera->ShaderData.reset(Shader::Create(vertexSource, fragmentSource));
		}
	}

	void OnUpdate(const float &deltaSeconds) override
	{
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
		RenderCommand::Clear();

		//WGINE_TRACE("Camera rotation: {0}, {1}, {2}", m_Camera.GetRotation().x, m_Camera.GetRotation().y, m_Camera.GetRotation().z);
		//WGINE_TRACE("Forward: {0}, {1}, {2}", m_Camera.GetForwardVector().x, m_Camera.GetForwardVector().y, m_Camera.GetForwardVector().z);
		//m_Square->SetLocation(m_Square->GetLocation() + glm::vec3(0.1f, 0.1f, 0.f) * deltaSeconds);

		//WGINE_CORE_TRACE("Delta time: {0} s, FPS: {1}", deltaSeconds, 1.f / deltaSeconds);
		auto speed = 5.f;
		if (Input::IsKeyPressed(WGINE_KEY_W))
			m_Camera.SetLocation(m_Camera.GetLocation() + m_Camera.GetForwardVector() * speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_S))
			m_Camera.SetLocation(m_Camera.GetLocation() + m_Camera.GetForwardVector() * -speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_D))
			m_Camera.SetLocation(m_Camera.GetLocation() + m_Camera.GetRightVector() * speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_A))
			m_Camera.SetLocation(m_Camera.GetLocation() + m_Camera.GetRightVector() * -speed * deltaSeconds);

		m_AxisCamera->SetTransform({
			{0.f, 0.f, 0.f},//m_Camera.GetLocation() + m_Camera.GetUpVector() * -0.1f,
			m_Camera.GetRotation(),
			m_Camera.GetScale()
			});

		Renderer::BeginScene(m_Camera); {

			Renderer::Submit(*m_Square);
			Renderer::Submit(*m_Triangle);
			Renderer::Submit(*m_Axis);
			Renderer::Submit(*m_AxisCamera);

		} Renderer::EndScene();
	}

	void OnEvent(Wgine::Event &event) override
	{
		EventDispatcher e = { event };
		e.Dispatch<MouseMovedEvent>(WGINE_BIND_EVENT_FN(ExampleLayer::OnMouseMoved));
	}
	
	bool OnMouseMoved(MouseMovedEvent &e)
	{
		//WGINE_TRACE("{0} {1}", e.GetPosition().x, e.GetPosition().y);
		auto pos = e.GetPosition();
		static bool initial = true;
		if (initial)
		{
			m_LastMousePosition = pos;
			initial = false;
		}
		auto delta = pos - m_LastMousePosition;
		m_LastMousePosition = pos;

		//WGINE_TRACE("Delta: {0} {1}", delta.x, delta.y);

		//auto deltaNormalized = delta / glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		m_Camera.SetRotation(m_Camera.GetRotation() + glm::vec3(0.f, delta.y, delta.x) * 0.05f);

		return true;
	}

private:
	std::unique_ptr<SceneEntity> m_Triangle;
	std::unique_ptr<SceneEntity> m_Square;
	std::unique_ptr<SceneEntity> m_Axis;
	std::unique_ptr<SceneEntity> m_AxisCamera;

	glm::vec2 m_LastMousePosition = glm::vec2(0.f);

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
