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
		m_Texture = TextureLibrary::Get("assets/textures/coords.png");
		m_TransparentTexture = TextureLibrary::Get("assets/textures/transparent.png");
		m_BaseMaterial = MakeRef<Material>(glm::vec3(1.f, 0.2f, 0.3f), 1.f, 0.f, 32.f, m_Texture, m_TransparentTexture);

		m_Scene = std::make_shared<Scene>();
		m_Camera = m_Scene->ConstructEntity<PerspectiveCamera>(Transform({ -3.f, -5.f, 2.f }), 45.f, 0.1f, 100000.f);
		Renderer::SetActiveCamera(m_Camera);
		//m_Scene->SetActiveCamera(m_Camera); // TODO: probably no need to ever store this
		auto controller = m_Scene->ConstructEntity<CameraController>(m_Camera);

		// floor
		auto floor = m_Scene->ConstructEntity<SceneEntity>();
		floor->ShaderData = ShaderLibrary::Get("assets/shaders/LitTexture.glsl");
		floor->MaterialData = MakeRef<Material>(glm::vec3(0.2f, 0.8f, 0.3f));
		floor->MeshData = MeshLibrary::GetQuad();
		floor->SetScale({ 100.f, 100.f, 1.f });

		// lighting
		auto ambientLight = m_Scene->ConstructLight<AmbientLight>(glm::vec3(1.f, 1.f, 1.f), 0.2f);
		auto diretionalLight = m_Scene->ConstructLight<DirectionalLight>(glm::vec3(0.f, -30.f, 0.f), glm::vec3(1.f, 1.f, 1.f), 0.5f);
		
		constexpr uint32_t pointLightCount = 3;
		glm::vec3 pointLightLocations[pointLightCount] = { glm::vec3(1.f, 3.f, 4.f), glm::vec3(-2.5f, -2.f, 1.f), glm::vec3(0.f, 4.f, 4.f) };
		glm::vec3 pointLightColors[pointLightCount] = { { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f } };
		for (uint32_t i = 0; i < pointLightCount; i++)
		{
			auto light = m_Scene->ConstructLight<PointLight>(pointLightLocations[i], pointLightColors[i], 1.f, 0.f, 4.f);
			auto lightVis = m_Scene->ConstructEntity<SceneEntity>(Transform(pointLightLocations[i]));
			lightVis->MeshData = MeshLibrary::GetSphere(16, 16);
			lightVis->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitDiffuseConst.glsl");
			lightVis->MaterialData = MakeRef<Material>(pointLightColors[i]);
			lightVis->SetScale({ 0.1f, 0.1f, 0.1f });
		}

		// TODO: correctly implement spot light radius (we need to find the intersection point behind the circle and thats gonna be the actual emission point), then we can properly calculate the light respecting the radius
		constexpr uint32_t spotLightCount = 3;
		glm::vec3 spotLightLocations[spotLightCount] = { glm::vec3(17.f, 13.f, 1.f), glm::vec3(-19.f, -12.f, 7.f), glm::vec3(-15.f, 20.f, 3.f) };
		glm::vec3 spotLightRotations[spotLightCount] = { glm::vec3(0.f, -90.f, 0.f), glm::vec3(0.f, -30.f, 180.f), glm::vec3(0.f, -60.f, 120.f) };
		glm::vec3 spotLightColors[spotLightCount] = { { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f } };
		for (uint32_t i = 0; i < spotLightCount; i++)
		{
			auto light = m_Scene->ConstructLight<SpotLight>(spotLightLocations[i], spotLightRotations[i], spotLightColors[i], 1.f, 0.f, 10.f, 45.f, 20.f);
			auto lightVis = m_Scene->ConstructEntity<SceneEntity>(Transform(spotLightLocations[i]));
			lightVis->MeshData = MeshLibrary::GetSphere(16, 16);
			lightVis->ShaderData = ShaderLibrary::Get("assets/shaders/UnlitDiffuseConst.glsl");
			lightVis->MaterialData = MakeRef<Material>(spotLightColors[i]);
			lightVis->SetScale({ 0.1f, 0.1f, 0.1f });
		}

		m_Cube = m_Scene->ConstructEntity<SceneEntity>();
		m_Cube->MeshData = MeshLibrary::GetCube(false);
		m_Cube->ShaderData = ShaderLibrary::Get("assets/shaders/LitTexture.glsl");
		m_Cube->MaterialData = MakeRef<Material>(glm::vec3(0.2f, 0.8f, 0.3f), 1.f, 0.4f, 128.f, TextureLibrary::Get("assets/textures/diffuse.png"), TextureLibrary::Get("assets/textures/specular.png"));
		m_Cube->SetLocation({ 3.f, 0.f, 0.5f });

		m_Sphere = m_Scene->ConstructEntity<SceneEntity>();
		m_Sphere->MeshData = MeshLibrary::GetSphere(); // 16, 16 for 4x better performance lol
		m_Sphere->ShaderData = ShaderLibrary::Get("assets/shaders/LitTexture.glsl");
		//m_Sphere->MaterialData = m_BaseMaterial;
		m_Sphere->SetLocation({ -2.f, 0.f, 2.f });

		m_Axis = m_Scene->ConstructEntity<SceneEntity>();
		m_AxisCamera = m_Scene->ConstructEntity<SceneEntity>();
		// axis data
		{
			auto axisMesh = MakeRef<Mesh>();
			m_Axis->MeshData = axisMesh;
			m_AxisCamera->MeshData = axisMesh;
			// red (x) front
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.0f,  0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 1.0f,  0.1f, 0.0f }, { 0.9f, 0.2f, 0.3f } },
				{ { 1.0f,  0.0f, 0.0f }, { 0.9f, 0.2f, 0.3f } }
			);
			// red (x) reverse
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 1.0f,  0.0f, 0.0f }, { 0.9f, 0.2f, 0.3f } },
				{ { 1.0f,  0.1f, 0.0f }, { 0.9f, 0.2f, 0.3f } },
				{ { 0.0f,  0.1f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
			);

			// green (y) front
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.0f,  0.0f, 0.1f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.0f,  1.0f, 0.1f }, { 0.5f, 0.8f, 0.2f } },
				{ { 0.0f,  1.0f, 0.0f }, { 0.5f, 0.8f, 0.2f } }
			);

			// green (y) reverse
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.0f,  1.0f, 0.0f }, { 0.5f, 0.8f, 0.2f } },
				{ { 0.0f,  1.0f, 0.1f }, { 0.5f, 0.8f, 0.2f } },
				{ { 0.0f,  0.0f, 0.1f }, { 0.0f, 0.0f, 0.0f } }
			);

			// blue (z) front
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.1f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.1f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f } },
				{ { 0.0f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f } }
			);

			// blue (z) reverse
			axisMesh->AddQuad(
				{ { 0.0f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
				{ { 0.0f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f } },
				{ { 0.1f,  0.0f, 1.0f }, { 0.2f, 0.5f, 0.9f } },
				{ { 0.1f,  0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
			);

			m_Axis->MeshData->RecalculateNormals();
			m_Axis->ShaderData = ShaderLibrary::Get("assets/shaders/VertexColor.glsl");
			m_AxisCamera->ShaderData = ShaderLibrary::Get("assets/shaders/VertexColor.glsl");
			m_Axis->SetLocation({ 0.f, 0.f, 0.1f });
		}
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

		
		Renderer::Submit(m_Scene.get());
		m_Sphere->DebugDrawNormals();
		m_Cube->DebugDrawNormals();
		m_Axis->DebugDrawNormals();
		Renderer::Flush();
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
