#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Wgine/Renderer/Mesh.h"
#include "Wgine/Core/Core.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;

	// TODO: put it in one file with vertex class
	static const BufferLayout s_VERTEX_LAYOUT = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		// TODO: texture id...
	};

	struct MeshInfo
	{
		MeshInfo(Ref<Mesh> mesh, const glm::mat4 &transform)
			: Mesh(mesh), Transform(transform)
		{}

		Ref<Mesh> Mesh;
		const glm::mat4 &Transform;
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
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
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
		WGINE_CORE_ASSERT(scene, "Invalid scene for renderer!");

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

				shaderData.Vertices.resize(shaderData.VertexCount);

				if (shaderData.VBO)
					shaderData.VBO->Unbind(); // TODO: necessary?
				shaderData.VBO = VertexBuffer::Create(sizeof(Vertex) * shaderData.VertexCount);
				shaderData.VBO->SetLayout(s_VERTEX_LAYOUT);
			}

			// resize index buffer
			if (shaderData.IndexCount > shaderData.CurrentMaxIndexCount)
			{
				shaderData.CurrentMaxIndexCount = shaderData.IndexCount;

				shaderData.Indices.resize(shaderData.IndexCount);

				if (shaderData.IBO)
					shaderData.IBO->Unbind(); // TODO: necessary?
				shaderData.IBO = IndexBuffer::Create(shaderData.IndexCount);
			}

			uint32_t vertexOffset = 0;
			uint32_t indexOffset = 0;
			for (const auto &meshData : shaderData.Meshes)
			{
				// TODO: avoid copying twice by copying directly to the vertices/indices buffer
				for (int i = vertexOffset; i < meshData.Mesh->GetVertices().size(); i++)
				{
					auto &vertex = meshData.Mesh->GetVertices()[i - vertexOffset];
					shaderData.Vertices[i] = Vertex(
						glm::vec3(meshData.Transform * glm::vec4(vertex.Position, 1.f)),
						vertex.Color,
						vertex.TexCoord);
				}

				for (int i = indexOffset; i < meshData.Mesh->GetIndices().size(); i++)
				{
					auto &index = meshData.Mesh->GetIndices()[i - indexOffset];
					shaderData.Indices[i] = index + vertexOffset;
				}

				vertexOffset += meshData.Mesh->GetVertices().size();
				indexOffset += meshData.Mesh->GetIndices().size();
			}

			shaderData.VBO->SetData(
				shaderData.Vertices.data(),
				sizeof(Vertex),
				shaderData.Vertices.size()
			);

			shaderData.IBO->SetData(
				shaderData.Indices.data(),
				shaderData.Indices.size()
			);

			shaderData.VAO->SetVertexBuffer(shaderData.VBO, 0);
			shaderData.VAO->SetIndexBuffer(shaderData.IBO);

			//shaderData.VAO->PrintDebug(1);
		}

		Flush();
	}

	void Renderer::Flush()
	{
		//auto &shaderData = s_ShaderData["VertexColor"];
		for (auto &[shaderName, shaderData] : s_ShaderData)
		{
			shaderData.Shader->Bind();
			shaderData.Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.ActiveScene->GetViewProjectionMatrix());
			//Shader->UploadUniformMat4("u_Transform", transform);
			shaderData.Shader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f }); // TODO: same thing as with transform; also the case with some other stuff
			shaderData.VAO->Bind();
			RenderCommand::DrawIndexed(shaderData.VAO, shaderData.IndexCount);
		}
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MeshData, entity.GetEntityMatrix());
	}

	void Renderer::Submit(const Ref<Shader> &shader, const Ref<Mesh> &mesh, const glm::mat4 &transform, std::function<void(const Ref<Shader>&)> submitExtraUniforms)
	{
		WGINE_ASSERT(s_RendererData.ActiveScene, "No active scene for renderer!");

		if (!mesh)
			return;

		// new Shader
		if (s_ShaderData.find(shader->GetName()) == s_ShaderData.end()) // TODO: when switched c++ 20 use .contains instead
		{
			s_ShaderData[shader->GetName()] = PerShaderData();
			s_ShaderData[shader->GetName()].Shader = shader;
		}

		auto &shaderData = s_ShaderData[shader->GetName()];

		shaderData.Meshes.push_back(MeshInfo(mesh, transform));
		shaderData.VertexCount += mesh->GetVertices().size();
		shaderData.IndexCount += mesh->GetIndices().size();
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
