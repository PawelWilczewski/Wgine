#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	Scene *Renderer::m_ActiveScene = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene(Scene *scene)
	{
		WGINE_ASSERT(scene, "Invalid scene for renderer!");
		m_ActiveScene = scene;

		for (auto entity : m_ActiveScene->m_SceneEntities) // should we acc do this here?
			Submit(*entity);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MeshData, entity.GetEntityMatrix());
	}

	void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, std::function<void(const Ref<Shader>&)> submitExtraUniforms)
	{
		WGINE_ASSERT(m_ActiveScene, "No active scene for renderer!");

		if (shader)
		{
			shader->Bind();
			shader->UploadUniformMat4("u_ViewProjection", m_ActiveScene->GetViewProjectionMatrix());
			shader->UploadUniformMat4("u_Transform", transform);
			submitExtraUniforms(shader);
		}

		if (vertexArray)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
