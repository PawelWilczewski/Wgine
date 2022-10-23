#include "Wgine.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

using namespace Wgine;

class UserInterfaceLayer : public Layer
{
public:
	UserInterfaceLayer()
		: Layer("UserInterface")
	{
		m_Scene = std::make_shared<Scene>();
		m_Camera = m_Scene->ConstructEntity<OrthographicCamera>(Transform());
		m_Scene->SetActiveCamera(m_Camera);
		auto controller = m_Scene->ConstructEntity<OrthographicCameraController2D>(m_Camera);

		m_TransparentTexture = TextureLibrary::Get("assets/textures/transparent.png");
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
		
		Renderer2D::BeginScene(m_Scene.get()); {
			Renderer2D::DrawQuad({ 1.f, 1.f }, 30.f, { 10.f, 10.f }, { 0.8f, 0.2f, 0.3f });
			Renderer2D::DrawQuad({ -2.f, -1.f }, 45.f, { 4.f, 4.f }, { 0.1f, 0.9f, 0.3f });
			Renderer2D::DrawQuad(Transform({ -20.f, 15.f, 10.f }, { 15.f, 0.f, 0.f }, { 10.f, 10.f, 10.f }), { 0.1f, 0.9f, 0.3f });
			Renderer2D::DrawQuad(Transform({ -20.f, -15.f, 5.f }, { 0.f, 0.f, 0.f }, { 10.f, 10.f, 10.f }), *m_TransparentTexture, { 2.f, 2.f }, { 0.8f, 0.3f, 0.4f, 0.8f });
		} Renderer2D::EndScene();
	}
	
	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Event &event) override
	{
		m_Scene->OnEvent(event);
	}

private:
	Ref<Scene> m_Scene;
	Ref<Texture2D> m_TransparentTexture;

	Camera *m_Camera;
};

class GameplayLayer : public Layer
{
public:
	GameplayLayer()
		: Layer("Gameplay")
	{
		m_Scene = std::make_shared<Scene>();
		m_Camera = m_Scene->ConstructEntity<PerspectiveCamera>(Transform({ -3.f, -5.f, 2.f }), 45.f, 0.1f, 100000.f);
		m_Scene->SetActiveCamera(m_Camera);
		auto controller = m_Scene->ConstructEntity<CameraController>(m_Camera);

		auto lightTransform = Transform({ 1.f, 3.f, 4.f });
		auto pointLight = m_Scene->ConstructLight<PointLight>(lightTransform, 1.0f, glm::vec3(0.2f, 0.8f, 0.8f));
		auto lightVis = m_Scene->ConstructEntity<SceneEntity>(lightTransform);
		lightVis->MeshData = MeshLibrary::GetSphere();
		lightVis->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitTexture.glsl");
		lightVis->SetScale({ 0.1f, 0.1f, 0.1f });

		m_Cube = m_Scene->ConstructEntity<SceneEntity>();
		m_Cube->MeshData = MeshLibrary::GetCubeSmooth();
		m_Cube->MeshData->RecalculateNormals(true);
		m_Cube->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitTexture.glsl");
		m_Cube->SetLocation({ 3.f, 0.f, 0.f });

		m_Sphere = m_Scene->ConstructEntity<SceneEntity>();
		m_Sphere->MeshData = MeshLibrary::GetSphere(16, 16);
		m_Sphere->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitTexture.glsl");
		m_Sphere->SetLocation({ -2.f, 0.f, 1.f });

		m_Axis = m_Scene->ConstructEntity<SceneEntity>();
		m_AxisCamera = m_Scene->ConstructEntity<SceneEntity>();
		m_AxisCamera->SetLocation({ 1.f, 2.f, 2.f });
		// axis data
		{
			auto axisMesh = MakeRef<Mesh>();
			m_Axis->MeshData = axisMesh;
			m_AxisCamera->MeshData = axisMesh;
			// triangle vertex buffer
			axisMesh->AddVertices({
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.f, 0.f } },
				{ { 0.0f,  0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.f, 0.f } }, 
				{ { 1.0f,  0.1f, 0.0f }, { 0.9f, 0.2f, 0.3f }, { 0.f, 0.f } }, // red
				{ { 1.0f,  0.0f, 0.0f }, { 0.9f, 0.2f, 0.3f }, { 0.f, 0.f } }, // red
				{ { 0.0f,  1.0f, 0.0f }, { 0.5f, 0.8f, 0.2f }, { 0.f, 0.f } }, // green
				{ { 0.0f,  1.0f, 0.1f }, { 0.5f, 0.8f, 0.2f }, { 0.f, 0.f } }, // green
				{ { 0.1f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.f, 0.f } }, 
				{ { 0.0f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f }, { 0.f, 0.f } }, // blue
				{ { 0.1f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f }, { 0.f, 0.f } }, // blue
				{ { 0.0f,  0.0f, 0.1f }, { 0.0f, 0.0f, 0.0f }, { 0.f, 0.f } },
			});
			axisMesh->AddIndices({
				0, 1, 2, 0, 2, 3,
				0, 4, 5, 5, 9, 0,
				0, 6, 7, 7, 8, 6
			});

			m_Axis->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitTexture.glsl");
			m_AxisCamera->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitTexture.glsl");
		}

		m_Texture = TextureLibrary::Get("assets/textures/coords.png");
		m_TransparentTexture = TextureLibrary::Get("assets/textures/transparent.png");

		m_BaseMaterial = MakeRef<Material>(glm::vec3(1.f, 0.2f, 0.3f), glm::vec3(1.f), glm::vec3(1.f), m_Texture, m_TransparentTexture);
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
		RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
		RenderCommand::Clear();

		m_AxisCamera->SetTransform({
			m_Camera->GetLocation() + m_Camera->GetForwardVector() * 0.2f + m_Camera->GetUpVector() * 0.04f + m_Camera->GetRightVector() * 0.10f,
			{ 0.f, 0.f, 0.f },
			{ 0.03f, 0.03f, 0.03f }
		});

		Renderer::BeginScene(m_Scene.get()); {

			m_Sphere->DebugDrawNormals();
			m_Cube->DebugDrawNormals();

		} Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text("FPS:        %f", Time::GetFPS());
		ImGui::Text("Frame time: %f ms", Time::GetDeltaSeconds() * 1000.f);
		ImGui::ColorEdit4("Shader color", glm::value_ptr(m_PickedColor));

		updates += 1.f;
		frameTimes += Time::GetDeltaSeconds() * 1000.f;
		ImGui::Text("Avg fps: %f", updates / Time::GetTimeSeconds());
		ImGui::Text("Avg frame time: %f", frameTimes / updates);

		ImGui::End();
	}

	void OnEvent(Event &event) override
	{
		m_Scene->OnEvent(event);
	}

private:
	float frameTimes = 0.f;
	float updates = 0.f;

	Ref<Scene> m_Scene;
	Camera *m_Camera;
	SceneEntity *m_Axis;
	SceneEntity *m_AxisCamera;
	SceneEntity *m_Sphere;
	SceneEntity *m_Cube;

	Ref<Material> m_BaseMaterial;

	Ref<Texture2D> m_Texture, m_TransparentTexture;

	glm::vec4 m_PickedColor = glm::vec4(0.5f, 0.2f, 0.8f, 1.f);
};

class Sandbox : public Wgine::Application
{
public:
	Sandbox()
	{
		PushLayer(new GameplayLayer());
		//PushOverlay(new UserInterfaceLayer());
	}

	~Sandbox()
	{

	}
};

Wgine::Application* Wgine::CreateApplication()
{
	return new Sandbox();
}
