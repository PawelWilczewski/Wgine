#include "WginePCH.h"
#include "Renderer.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	Scene *Renderer::m_ActiveScene = nullptr;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Scene *scene)
	{
		m_ActiveScene = scene;
		WGINE_ASSERT(m_ActiveScene, "Invalid scene for renderer!");

		for (auto entity : scene->m_SceneEntities) // should we acc do this here?
			Submit(*entity);
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MeshData, entity.GetEntityMatrix());
	}

	void Renderer::Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform)
	{
		WGINE_ASSERT(m_ActiveScene, "No active scene for renderer!");

		if (shader)
		{
			shader->Bind();
			shader->UploadUniformMat4("u_ViewProjection", m_ActiveScene->GetViewProjectionMatrix());
			shader->UploadUniformMat4("u_Transform", transform);
		}

		if (vertexArray)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}
}
