#include "WginePCH.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

#include "Wgine/Renderer/Shader.h"
#include "Wgine/Renderer/ShaderLibrary.h"
#include "Wgine/Core/Transform.h"
#include "Wgine/Core/Time.h"

namespace Wgine
{
	struct TriVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO: texture id...
	};

	struct RendererData
	{
		const uint32_t MaxTrisPerCall = 20000;
		const uint32_t MaxVertsPerCall = MaxTrisPerCall * 3;
		const uint32_t MaxIndicesPerCall = MaxTrisPerCall * 3;

		Ref<VertexArray> TriVA;
		Ref<VertexBuffer> TriVB;
		Ref<Shader> UnlitTextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t TriIndexCount = 0;
		Scope<TriVertex[]> TriVertexBufferStart = nullptr;
		TriVertex *TriVertexBuffer = nullptr;

		//Scope<glm::mat4[]> Transforms = MakeScope<glm::mat4[]>(s_Data.MaxVertsPerCall);

		Scene *ActiveScene = nullptr;
	};

	static RendererData s_Data;

	void Renderer2D::Init()
	{
		// Quad Vertex Array
		// vertex buffer
		s_Data.TriVA = VertexArray::Create();
		s_Data.TriVB = VertexBuffer::Create(sizeof(TriVertex), s_Data.MaxVertsPerCall);
		s_Data.TriVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		s_Data.TriVA->AddVertexBuffer(s_Data.TriVB);
		
		s_Data.TriVertexBufferStart = MakeScope<TriVertex[]>(s_Data.MaxVertsPerCall);

		auto triIndices = MakeScope<uint32_t[]>(s_Data.MaxIndicesPerCall);
		// index buffer
		//unsigned int quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		for (uint32_t i = 0; i < s_Data.MaxIndicesPerCall; i++)
			triIndices[i] = i;
		auto triIB = IndexBuffer::Create(triIndices.get(), s_Data.MaxIndicesPerCall);
		s_Data.TriVA->SetIndexBuffer(triIB);

		//s_Data.Transforms = MakeScope<glm::mat4[]>(s_Data.MaxVertsPerCall);

		s_Data.UnlitTextureShader = Shader::Create("assets/shaders/UnlitTexture.glsl");
		s_Data.UnlitTextureShader->Bind();
		s_Data.UnlitTextureShader->UploadUniformInt("u_Texture", 0);

		uint32_t whiteData = 0xffffffff;
		s_Data.WhiteTexture = Texture2D::Create(1, 1, &whiteData);
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(Scene *scene)
	{
		s_Data.ActiveScene = scene;
		s_Data.TriIndexCount = 0;
		s_Data.TriVertexBuffer = s_Data.TriVertexBufferStart.get();
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = s_Data.TriVertexBuffer - s_Data.TriVertexBufferStart.get();
		s_Data.TriVB->SetData(s_Data.TriVertexBufferStart.get(), sizeof(TriVertex), dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.TriVA, s_Data.TriIndexCount);
	}

	static void Submit(const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, std::function<void(const Ref<Shader> &)> submitExtraUniforms = [&](const Ref<Shader> &) {})
	{
		WGINE_ASSERT(s_Data->ActiveScene, "No active scene for renderer!");

		s_Data.UnlitTextureShader->UploadUniformMat4("u_ViewProjection", s_Data.ActiveScene->GetViewProjectionMatrix());
		s_Data.UnlitTextureShader->UploadUniformMat4("u_Transform", transform);
		s_Data.UnlitTextureShader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f });
		submitExtraUniforms(s_Data.UnlitTextureShader);

		if (vertexArray)
		{
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const glm::vec4 &color)
	{
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const Texture2D &texture, const glm::vec2 &tiling, const glm::vec4 &tint)
	{
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), texture, tiling, tint);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec4 &color)
	{
		WGINE_CORE_ASSERT(s_Data->ActiveScene, "Invalid active scene when creating quad!");

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f,  0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 0.f, 0.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f, -0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 0.f, 1.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f, -0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 1.f, 1.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f,  0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 0.f, 0.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f, -0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 1.f, 1.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriVertexBuffer->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f,  0.5f, 1.f);
		s_Data.TriVertexBuffer->Color = color;
		s_Data.TriVertexBuffer->TexCoord = { 1.f, 0.f };
		s_Data.TriVertexBuffer++;

		s_Data.TriIndexCount += 6;

		/*s_Data.WhiteTexture->Bind();
		Submit(s_Data.TriVA, transform.ToModelMatrix(), [&](Ref<Shader> s) {
			s->UploadUniformFloat4("u_Color", color);
			});*/
	}

	void Renderer2D::DrawQuad(const Transform &transform, const Texture2D &texture, const glm::vec2 &tiling, const glm::vec4 &tint)
	{
		WGINE_CORE_ASSERT(s_Data->ActiveScene, "Invalid active scene when creating quad!");
		texture.Bind();
		Submit(s_Data.TriVA, transform.ToModelMatrix(), [&](Ref<Shader> s) {
			s->UploadUniformFloat4("u_Color", tint);
			s->UploadUniformFloat2("u_Tiling", tiling);
			});
	}
}
