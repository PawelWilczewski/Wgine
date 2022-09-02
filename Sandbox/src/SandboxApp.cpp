#include "Wgine.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

using namespace Wgine;

class ExampleLayer : public Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		m_Scene = std::make_shared<Scene>();
		m_Camera = m_Scene->ConstructEntity<PerspectiveCamera>(Transform({ -3.f, -5.f, 2.f }), 45.f, 1600, 900, 0.1f, 100000.f);
		//m_Camera = PerspectiveCamera(Transform({ -3.f, -5.f, 2.f }), 45.f, 1600, 900, 0.1f, 100000.f);
		//m_Camera = OrthographicCamera(Transform(), -1.6f, 1.6f, -0.9f, 0.9f);

		m_Triangle = m_Scene->ConstructEntity<SceneEntity>();
		// triangle data
		{
			m_Triangle->MeshData.reset(VertexArray::Create());
			// triangle vertex buffer
			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.1f, 0.2f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.4f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.2f, 0.5f, 0.9f, 1.0f,
			};
			Ref<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				});
			m_Triangle->MeshData->AddVertexBuffer(vertexBuffer);

			// triangle index buffer
			unsigned int indices[3] = { 0, 1, 2 };
			Ref<IndexBuffer> indexBuffer;
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

			m_Triangle->ShaderData = Shader::Create("VertexPosition", vertexSource, fragmentSource);
		}

		m_Square = m_Scene->ConstructEntity<SceneEntity>();
		m_Square->SetRotation({ 0.f, 45.f, 180.f });
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
			Ref<VertexBuffer> squareVertexBuffer;
			squareVertexBuffer.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
			squareVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				});
			m_Square->MeshData->AddVertexBuffer(squareVertexBuffer);

			// square indices
			unsigned int squareIndices[12] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
			Ref<IndexBuffer> squareIndexBuffer;
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

			m_Square->ShaderData = Shader::Create("SquareColor", squareVertexSource, squareFragmentSource);
		}

		m_Axis = m_Scene->ConstructEntity<SceneEntity>();
		m_AxisCamera = m_Scene->ConstructEntity<SceneEntity>();
		m_AxisCamera->SetLocation({ 1.f, 2.f, 2.f });
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
			Ref<VertexBuffer> vertexBuffer;
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
			Ref<IndexBuffer> indexBuffer;
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

			m_Axis->ShaderData = Shader::Create("VertexColor", vertexSource, fragmentSource);
			m_AxisCamera->ShaderData = Shader::Create("VertexColor", vertexSource, fragmentSource);
		}

		m_VertexArray.reset(VertexArray::Create());
		// vertex buffer
		float verticesSquare[5 * 4] = {
			-1.0f,  1.0f, 0.f, 1.f, 1.f,
			-1.0f, -1.0f, 0.f, 0.f, 1.f,
			1.0f,  -1.0f, 0.f, 0.f, 0.f,
			1.0f,   1.0f, 0.f, 1.f, 0.f,
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(verticesSquare, sizeof(verticesSquare)));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		// index buffer
		unsigned int indices[12] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_FlatShader = Shader::Create("assets/shaders/VertexColor.glsl");

		m_TextureShader = Shader::Create("assets/shaders/Texture.glsl");

		m_Texture = Texture2D::Create("assets/textures/coords.png");
		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);

		m_TransparentTexture = Texture2D::Create("assets/textures/transparent.png");
	}

	virtual void OnAttach() override
	{
		m_Scene->OnStart();
	}

	virtual void OnDetach() override
	{
		m_Scene->OnEnd();
	}

	virtual void OnUpdate(const float &deltaSeconds) override
	{
		m_Scene->OnTick(deltaSeconds);
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1 });
		RenderCommand::Clear();

		//WGINE_CORE_TRACE("Delta time: {0} s, FPS: {1}", deltaSeconds, 1.f / deltaSeconds);
		auto speed = 5.f;
		if (Input::IsKeyPressed(WGINE_KEY_W))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetForwardVector() * speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_S))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetForwardVector() * -speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_D))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetRightVector() * speed * deltaSeconds);
		if (Input::IsKeyPressed(WGINE_KEY_A))
			m_Camera->SetLocation(m_Camera->GetLocation() + m_Camera->GetRightVector() * -speed * deltaSeconds);

		m_AxisCamera->SetTransform({
			m_Camera->GetLocation() + m_Camera->GetForwardVector() * 0.2f + m_Camera->GetUpVector() * 0.04f + m_Camera->GetRightVector() * 0.10f,
			{ 0.f, 0.f, 0.f },
			{ 0.03f, 0.03f, 0.03f }
			});

		Renderer::BeginScene(*m_Camera); {

			Renderer::Submit(*m_Square);
			Renderer::Submit(*m_Triangle);
			Renderer::Submit(*m_Axis);
			Renderer::Submit(*m_AxisCamera);

			for (int y = 0; y < 10; y++)
			{
				for (int x = 0; x < 10; x++)
				{
					auto modelMatrix = Transform(glm::vec3(3.f + 2.5f * x, 3.f + 2.5f * y, 0.f)).ToModelMatrix();
					m_FlatShader->UploadUniformFloat4("u_Color", y % 2 == 1 ? m_PickedColor : glm::vec4(.4f, 0.8f, 0.3f, 1.f));
					Renderer::Submit(m_FlatShader, m_VertexArray, modelMatrix);
				}
			}

			m_Texture->Bind();
			Renderer::Submit(m_TextureShader, m_VertexArray, Transform({ 5.f, -6.f, 2.f }, { 0.f, -90.f, 0.f }, { 5.f, 5.f, 5.f }).ToModelMatrix());
			m_TransparentTexture->Bind();
			Renderer::Submit(m_TextureShader, m_VertexArray, Transform({ 0.f, -10.f, 2.f }, { 0.f, -90.f, -90.f }, { 5.f, 5.f, 5.f }).ToModelMatrix());
		} Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::ColorEdit4("Shader color", glm::value_ptr(m_PickedColor));
		ImGui::End();
	}

	void OnEvent(Wgine::Event &event) override
	{
		m_Scene->OnEvent(event);
		EventDispatcher e = { event };
		e.Dispatch<MouseMovedEvent>(WGINE_BIND_EVENT_FN(ExampleLayer::OnMouseMoved));
		e.Dispatch<MouseButtonPressedEvent>(WGINE_BIND_EVENT_FN(ExampleLayer::OnMouseButtonPressed));
	}
	
	bool OnMouseMoved(MouseMovedEvent &e)
	{
		if (Application::Get().GetWindow().GetShowMouse()) // TODO: obv better api and this is blatantly wrong??? shouldn't be negation
			return false;

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

		// TODO: should we do something like this to ensure the same rotation always if the same physical movement was performed? regardless of the current res
		//auto deltaNormalized = delta / glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		m_Camera->SetRotation(m_Camera->GetRotation() + glm::vec3(0.f, delta.y, delta.x) * 0.05f);

		return false;
	}

	bool OnMouseButtonPressed(MouseButtonPressedEvent &e)
	{
		auto window = &Application::Get().GetWindow();
		if (e.GetMouseButton() == WGINE_MOUSE_BUTTON_RIGHT)
			window->SetShowMouse(!window->GetShowMouse());
		return true;
	}

private:
	Ref<Scene> m_Scene;
	SceneEntity *m_Triangle;
	SceneEntity *m_Square;
	SceneEntity *m_Axis;
	SceneEntity *m_AxisCamera;
	Camera *m_Camera;

	Ref<Texture2D> m_Texture, m_TransparentTexture;

	Ref<VertexArray> m_VertexArray;
	Ref<Shader> m_FlatShader, m_TextureShader;

	glm::vec4 m_PickedColor = glm::vec4(0.5f, 0.2f, 0.8f, 1.f);

	glm::vec2 m_LastMousePosition = glm::vec2(0.f);
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
