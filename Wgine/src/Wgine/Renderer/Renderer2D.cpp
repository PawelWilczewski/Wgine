#include "WginePCH.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

#include "Renderer.h"

#include "Wgine/Renderer/Shader.h"
#include "Wgine/Renderer/ShaderLibrary.h"
#include "Wgine/Core/Transform.h"
#include "Wgine/Core/Time.h"

#include "Wgine/Renderer/Mesh.h"
#include "Wgine/Renderer/Vertex.h"
#include "Wgine/Renderer/TextureLibrary.h"

namespace Wgine
{
	//std::unordered_map<std::string, Ref<Shader>> s_Shaders;

	//struct RendererData
	//{
	//	const uint32_t MaxTrisPerCall = 20000;
	//	const uint32_t MaxVertsPerCall = MaxTrisPerCall * 3;
	//	const uint32_t MaxIndicesPerCall = MaxTrisPerCall * 3;

	//	Ref<VertexArray> VAO;
	//	Ref<VertexBuffer> VBO;
	//	Ref<IndexBuffer> IBO;
	//	Ref<Shader> UnlitTextureShader;

	//	uint32_t IndexCount = 0;
	//	Unique<Vertex[]> VBBase = nullptr;
	//	Vertex *VB = nullptr;

	//	//Unique<glm::mat4[]> Transforms = MakeUnique<glm::mat4[]>(s_Data.MaxVertsPerCall);

	//	Scene *ActiveScene = nullptr;
	//};

	//static RendererData s_Data;

	void Renderer2D::Init()
	{
		//// Quad Vertex Array
		//// vertex buffer
		//s_Data.VAO = VertexArray::Create();
		//s_Data.VBO = VertexBuffer::Create(sizeof(Vertex) * s_Data.MaxVertsPerCall);
		//s_Data.VBO->SetLayout(Vertex::GetLayout());
		//s_Data.VAO->AddVertexBuffer(s_Data.VBO);
		//
		//s_Data.VBBase = MakeUnique<Vertex[]>(s_Data.MaxVertsPerCall);

		//auto indices = MakeUnique<uint32_t[]>(s_Data.MaxIndicesPerCall);
		//// index buffer
		////unsigned int quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		//for (uint32_t i = 0; i < s_Data.MaxIndicesPerCall; i++)
		//	indices[i] = i;
		//s_Data.IBO = IndexBuffer::Create(indices.get(), s_Data.MaxIndicesPerCall);
		//s_Data.VAO->SetIndexBuffer(s_Data.IBO);

		////s_Data.Transforms = MakeUnique<glm::mat4[]>(s_Data.MaxVertsPerCall);

		//s_Data.UnlitTextureShader = ShaderLibrary::Get("assets/shaders/LitTexture.glsl");
	}

	void Renderer2D::Shutdown()
	{
		
	}

	void Renderer2D::BeginScene(Scene *scene)
	{
		//s_Data.ActiveScene = scene;
		//s_Data.IndexCount = 0;
		//s_Data.VB = s_Data.VBBase.get();
	}

	void Renderer2D::EndScene()
	{
		//uint32_t dataSize = s_Data.VB - s_Data.VBBase.get();
		//s_Data.VBO->SetData(s_Data.VBBase.get(), sizeof(Vertex) * dataSize);

		//Flush();
	}

	void Renderer2D::Flush()
	{
		//TextureLibrary::GetTransparent()->Bind(1);

		//s_Data.UnlitTextureShader->Bind();
		//s_Data.UnlitTextureShader->UploadUniformMat4("u_ViewProjection", s_Data.ActiveScene->GetViewProjectionMatrix());
		//s_Data.UnlitTextureShader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f }); // TODO: same thing as with transform; also the case with some other stuff
		//s_Data.UnlitTextureShader->UploadUniformIntArray("u_Texture", Renderer:: s_TextureSlots, Renderer::s_TextureSlotsCount);

		//s_Data.VAO->Bind();
		//s_Data.VBO->Bind();
		//s_Data.IBO->Bind();
		//
		//RenderCommand::DrawIndexed(s_Data.VAO, s_Data.IndexCount);
	}

	static void Submit(const Ref<VertexArray> &vertexArray, const glm::mat4 &transform, std::function<void(const Ref<Shader> &)> submitExtraUniforms = [&](const Ref<Shader> &) {})
	{
		//WGINE_ASSERT(s_Data.ActiveScene, "No active scene for renderer!");

		//s_Data.UnlitTextureShader->UploadUniformMat4("u_ViewProjection", s_Data.ActiveScene->GetViewProjectionMatrix());
		//s_Data.UnlitTextureShader->UploadUniformMat4("u_Transform", transform);
		//s_Data.UnlitTextureShader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f });
		//submitExtraUniforms(s_Data.UnlitTextureShader);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const glm::vec3 &color)
	{
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), color);
	}

	void Renderer2D::DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, Texture2D &texture, const glm::vec2 &tiling, const glm::vec4 &tint)
	{
		DrawQuad(Transform(glm::vec3(-1.f, location.x, location.y), glm::vec3(rotation, 0.f, 0.f), glm::vec3(1.f, scale.x, scale.y)), texture, tiling, tint);
	}

	void Renderer2D::DrawQuad(const Transform &transform, const glm::vec3 &color)
	{
		//WGINE_CORE_ASSERT(s_Data.ActiveScene, "Invalid active scene when creating quad!");

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f,  0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 0.f, 0.f };
		//s_Data.VB++;

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f, -0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 0.f, 1.f };
		//s_Data.VB++;

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f, -0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 1.f, 1.f };
		//s_Data.VB++;

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,  -0.5f,  0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 0.f, 0.f };
		//s_Data.VB++;

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f, -0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 1.f, 1.f };
		//s_Data.VB++;

		//s_Data.VB->Position = transform.ToModelMatrix() * glm::vec4(0.f,   0.5f,  0.5f, 1.f);
		//s_Data.VB->Color = color;
		//s_Data.VB->TexCoord = { 1.f, 0.f };
		//s_Data.VB++;

		//s_Data.IndexCount += 6;

		///*s_Data.WhiteTexture->Bind();
		//Submit(s_Data.VAO, transform.ToModelMatrix(), [&](Ref<Shader> s) {
		//	s->UploadUniformFloat4("u_Color", color);
		//	});*/
	}

	void Renderer2D::DrawQuad(const Transform &transform, Texture2D &texture, const glm::vec2 &tiling, const glm::vec4 &tint)
	{
		//WGINE_CORE_ASSERT(s_Data.ActiveScene, "Invalid active scene when creating quad!");
		//texture.Bind();
		////Submit(s_Data.VAO, transform.ToModelMatrix(), [&](Ref<Shader> s) {
		////	s->UploadUniformFloat4("u_Color", tint);
		////	s->UploadUniformFloat2("u_Tiling", tiling);
		////	});
	}
}
