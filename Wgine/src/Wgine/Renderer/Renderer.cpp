#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Wgine/Renderer/Mesh.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	
	//struct TriVertex
	//{
	//	glm::vec3 Position;
	//	glm::vec4 Color;
	//	glm::vec2 TexCoord;
	//	// TODO: texture id...
	//};

	static const BufferLayout s_VertexLayout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexCoord" },
	};

	struct MeshInfo
	{
		MeshInfo(Ref<Mesh> mesh, const glm::mat4 &transform, uint32_t vertexOffset, uint32_t indexOffset)
			: Mesh(mesh), Transform(transform), VertexOffset(vertexOffset), IndexOffset(indexOffset)
		{}

		Ref<Mesh> Mesh;
		const glm::mat4 &Transform;
		uint32_t VertexOffset = 0;
		uint32_t IndexOffset = 0;
	};

	struct PerShaderData
	{
		PerShaderData()
		{}

		Ref<Shader> Shader;
		Ref<VertexArray> VAO = VertexArray::Create();
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;

		uint32_t IndexCount = 0;
		uint32_t CurrentMaxIndexCount = 0;
		uint32_t VertexCount = 0;
		uint32_t CurrentMaxVertexCount = 0;

		std::vector<MeshInfo> Meshes = std::vector<MeshInfo>();

		//std::vector<Ref<Mesh>> Meshes = std::vector<Ref<Mesh>>();

		//Scope<Vertex[]> VertexBufferStart = nullptr;
		//Vertex *VertexBufferTop = nullptr;

		//Scope<glm::mat4[]> Transforms = MakeScope<glm::mat4[]>(s_Data.MaxVertsPerCall);
	};

	static std::unordered_map<std::string, PerShaderData> s_ShaderData;

	struct RendererData
	{
		//const uint32_t MaxTrisPerCall = 20000;
		//const uint32_t MaxVertsPerCall = MaxTrisPerCall * 3;
		//const uint32_t MaxIndicesPerCall = MaxTrisPerCall * 3;
		Scene *ActiveScene = nullptr;
	};

	static RendererData s_RendererData = RendererData();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();

		s_ShaderData = std::unordered_map<std::string, PerShaderData>();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::BeginScene(Scene *scene)
	{
		WGINE_ASSERT(scene, "Invalid scene for renderer!");

		s_RendererData.ActiveScene = scene;

		// reset shader data
		for (auto &[shaderName, shaderData] : s_ShaderData)
		{
			shaderData.IndexCount = 0;
			shaderData.VertexCount = 0;
			shaderData.Meshes.clear();
		}

		for (auto entity : s_RendererData.ActiveScene->m_SceneEntities)
			Submit(*entity);
	}

	void Renderer::EndScene()
	{
		for (auto &[shaderName, shaderData] : s_ShaderData)
		{
			WGINE_CORE_TRACE("Shader {0}:", shaderName);
			// resize vertex buffer
			if (shaderData.VertexCount > shaderData.CurrentMaxVertexCount)
			{
				shaderData.CurrentMaxVertexCount = shaderData.VertexCount;
				shaderData.VBO = VertexBuffer::Create(sizeof(Vertex), shaderData.VertexCount);
				shaderData.VBO->SetLayout(s_VertexLayout);
			}

			// resize index buffer
			if (shaderData.IndexCount > shaderData.CurrentMaxIndexCount)
			{
				shaderData.CurrentMaxIndexCount = shaderData.IndexCount;
				shaderData.IBO = IndexBuffer::Create(shaderData.IndexCount);
			}

			for (const auto &meshData : shaderData.Meshes)
			{
				// transform the positions
				shaderData.VBO->SetData(
					meshData.Mesh->GetVerticesTransformed(meshData.Transform).data(),
					sizeof(Vertex),
					meshData.Mesh->GetVertices().size(),
					meshData.VertexOffset
				);

				// offset the indices appropriately
				auto indices = meshData.Mesh->GetIndicesOffset(meshData.IndexOffset);
				shaderData.IBO->SetData(
					indices.data(),
					meshData.Mesh->GetIndices().size(),
					meshData.IndexOffset
				);
			}

			shaderData.VAO->InsertVertexBuffer(shaderData.VBO, 0);
			shaderData.VAO->SetIndexBuffer(shaderData.IBO);

			shaderData.VAO->PrintDebug(1);
		}

		Flush();
	}

	void Renderer::Flush()
	{
		for (auto &[shaderName, shaderData] : s_ShaderData)
		{
			shaderData.Shader->Bind();
			shaderData.Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.ActiveScene->GetViewProjectionMatrix());
			//Shader->UploadUniformMat4("u_Transform", transform);
			shaderData.Shader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f }); // TODO: same thing as with transform; also the case with some other stuff
			RenderCommand::DrawIndexed(shaderData.VAO);
		}
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MeshData, entity.GetEntityMatrix());
	}

	void Renderer::Submit(const Ref<Shader> &shader, const Ref<Mesh> &mesh, const glm::mat4 &transform, std::function<void(const Ref<Shader>&)> submitExtraUniforms)
	{
		WGINE_ASSERT(m_ActiveScene, "No active scene for renderer!");

		if (!mesh)
			return;

		// new Shader
		if (s_ShaderData.find(shader->GetName()) == s_ShaderData.end()) // TODO: when switched c++ 20 use .contains instead
		{
			s_ShaderData[shader->GetName()] = PerShaderData();
			s_ShaderData[shader->GetName()].Shader = shader;
		}

		auto &shaderData = s_ShaderData[shader->GetName()];

		shaderData.Meshes.push_back(MeshInfo(mesh, transform, shaderData.VertexCount, shaderData.IndexCount));
		shaderData.VertexCount += mesh->GetVertices().size();
		shaderData.IndexCount += mesh->GetIndices().size();
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
