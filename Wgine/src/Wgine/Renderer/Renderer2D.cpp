#include "WginePCH.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

#include "Wgine/Renderer/Shader.h"
#include "Wgine/Renderer/ShaderLibrary.h"
#include "Wgine/Core/Transform.h"
#include "Wgine/Core/Time.h"

namespace Wgine
{
	struct PrimitiveData
	{
		Ref<VertexArray> QuadVA;
		Ref<Shader> UnlitShader;
		Ref<Shader> TextureShader;
		Scene *ActiveScene = nullptr;
	};

	static PrimitiveData *data;

	void Renderer2D::Init()
	{
		data = new PrimitiveData();

		// Quad Vertex Array
		// vertex buffer
		data->QuadVA = VertexArray::Create();
		float quadVertices[5 * 4] = {
			0.f,  -0.5f,  0.5f,  0.f, 0.f,
			0.f,  -0.5f, -0.5f,  0.f, 1.f,
			0.f,   0.5f, -0.5f,  1.f, 1.f,
			0.f,   0.5f,  0.5f,  1.f, 0.f,
		};
		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		data->QuadVA->AddVertexBuffer(vertexBuffer);

		// index buffer
		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		data->QuadVA->SetIndexBuffer(indexBuffer);

		data->UnlitShader = Shader::Create("assets/shaders/Unlit.glsl");
		data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete data;
	}

	void Renderer2D::BeginScene(Scene *scene)
	{
		data->ActiveScene = scene;
	}

	void Renderer2D::EndScene()
	{
	}

	static void Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, std::function<void(Ref<Shader>)> submitExtraUniforms = [](Ref<Shader>) {})
	{
		WGINE_ASSERT(data->ActiveScene, "No active scene for renderer!");

		if (shader)
		{
			shader->Bind();
			shader->UploadUniformMat4("u_ViewProjection", data->ActiveScene->GetViewProjectionMatrix());
			shader->UploadUniformMat4("u_Transform", transform);
			submitExtraUniforms(shader);
		}

		if (vertexArray)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const glm::vec4 &color)
	{
		WGINE_CORE_ASSERT(data->ActiveScene, "Invalid active scene when creating quad!");
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const Texture2D &texture)
	{
		WGINE_CORE_ASSERT(data->ActiveScene, "Invalid active scene when creating quad!");
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), texture);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec4 &color)
	{
		WGINE_CORE_ASSERT(data->ActiveScene, "Invalid active scene when creating quad!");
		Submit(data->UnlitShader, data->QuadVA, transform.ToModelMatrix(), [&](Ref<Shader> s) {
			s->UploadUniformFloat4("u_Color", color);
			});
	}

	void Renderer2D::DrawQuad(const Transform &transform, const Texture2D &texture)
	{
		WGINE_CORE_ASSERT(data->ActiveScene, "Invalid active scene when creating quad!");
		texture.Bind();
		Submit(data->TextureShader, data->QuadVA, transform.ToModelMatrix());
	}
}
