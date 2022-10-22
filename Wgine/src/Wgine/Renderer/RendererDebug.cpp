#include "WginePCH.h"
#include "RendererDebug.h"

#include "ShaderLibrary.h"
#include "RenderCommand.h"

namespace Wgine
{
	struct RendererData
	{
		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<Shader> Shader;
		std::vector<Vertex> LineVerts;
		Camera *Camera = nullptr;
	};

	static RendererData s_RendererData;

	void RendererDebug::SetCamera(Camera *camera)
	{
		s_RendererData.Camera = camera;
	}

	void RendererDebug::Init()
	{
		s_RendererData.VAO = VertexArray::Create();
		s_RendererData.VBO = VertexBuffer::Create(sizeof(Vertex) * 100);
		s_RendererData.VBO->SetLayout(Vertex::GetLayout());
		s_RendererData.VAO->AddVertexBuffer(s_RendererData.VBO);

		s_RendererData.Shader = ShaderLibrary::Get("assets/shaders/Unlit.glsl");

		s_RendererData.LineVerts = std::vector<Vertex>();
	}

	void RendererDebug::DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec3 &color)
	{
		s_RendererData.LineVerts.push_back(Vertex(from, color));
		s_RendererData.LineVerts.push_back(Vertex(to, color));
	}

	void RendererDebug::Flush()
	{
		// upload data
		s_RendererData.VBO->SetData(s_RendererData.LineVerts.data(), sizeof(Vertex) * s_RendererData.LineVerts.size());

		// bind shader and upload uniforms
		s_RendererData.Shader->Bind();
		s_RendererData.Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.Camera->GetViewProjectionMatrix());

		// draw
		s_RendererData.VAO->Bind();
		s_RendererData.VBO->Bind();
		RenderCommand::DrawLines(s_RendererData.LineVerts.size());

		// reset
		s_RendererData.LineVerts.clear();
	}
}
