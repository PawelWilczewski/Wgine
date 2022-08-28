#include "WginePCH.h"
#include "Renderer.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;

	Renderer::SceneData *Renderer::m_SceneData = new SceneData();

	void Renderer::BeginScene(Camera &camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MeshData, entity.GetEntityMatrix());
	}

	void Renderer::Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray, const glm::mat4 &transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
