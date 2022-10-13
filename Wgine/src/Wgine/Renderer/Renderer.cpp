#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Vertex.h"

#include "Wgine/Renderer/Mesh.h"
#include "Wgine/Renderer/Vertex.h"
#include "Wgine/Core/Core.h"

#include "Wgine/Renderer/Material.h"


namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;

	const uint32_t Renderer::s_TextureSlotsCount = 32;
	int Renderer::s_TextureSlots[s_TextureSlotsCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

	constexpr uint32_t MaterialsBindingSlot = 0;
	
	struct MeshInfo
	{
		MeshInfo(Ref<Mesh> mesh, Ref<glm::mat4> transform)
			: Mesh(mesh), Transform(transform)
		{}

		Ref<Mesh> Mesh;
		// TODO: in rare occasions, the transform might be invalidated before we render (object destroyed on a different thread or sth?), possibly we will have to use Ref<Transform> instead
		Ref<glm::mat4> Transform;
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

		Ref<StorageBuffer> MaterialSSBO;
		// material index in the ssbo array for each according vertex index
		// TODO: we can probably include transforms in the "material/mesh info?" ssbo to see if it's faster to carry out transform * viewprojection multiplication on the gpu
		std::vector<Ref<PhongMaterial>> Materials; // TODO: in the future we can simplify the SSBO of materials to exclude duplicates and save assets (if ref is the same then no need to copy to ssbo; just adjust indices appropriately)
		uint32_t CurrentMaxMaterialCount = 0;

		Ref<StorageBuffer> MaterialIDSSBO;
		std::vector<int> MaterialID;
	};

	static std::unordered_map<std::string, PerShaderData> s_ShaderData;

	struct RendererData
	{
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
			shaderData.Meshes.clear(); // TODO: instead maybe just use count so no need to resize every time added
			shaderData.Materials.clear(); // TODO: same here
		}

		for (auto entity : s_RendererData.ActiveScene->m_SceneEntities)
			Submit(*entity);
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MaterialData, entity.MeshData, MakeRef<glm::mat4>(entity.GetEntityMatrix()));
	}

	void Renderer::Submit(Ref<Shader> shader, Ref<PhongMaterial> material, Ref<Mesh> mesh, Ref<glm::mat4> transform)
	{
		WGINE_ASSERT(s_RendererData.ActiveScene, "No active scene for renderer!");

		if (!mesh)
			return;

		// new Shader
		if (s_ShaderData.find(shader->GetPath()) == s_ShaderData.end()) // TODO: when switched c++ 20 use .contains instead
		{
			s_ShaderData[shader->GetPath()] = PerShaderData();
			s_ShaderData[shader->GetPath()].Shader = shader;
		}

		auto &shaderData = s_ShaderData[shader->GetPath()];

		shaderData.Meshes.push_back(MeshInfo(mesh, transform));
		shaderData.VertexCount += mesh->GetVertices().size();
		shaderData.IndexCount += mesh->GetIndices().size();

		shaderData.Materials.push_back(material);
	}

	void Renderer::EndScene()
	{
		for (auto &[shaderName, shaderData] : s_ShaderData)
		{
			// resize vertex buffer and material ids buffer
			if (shaderData.VertexCount > shaderData.CurrentMaxVertexCount)
			{
				shaderData.CurrentMaxVertexCount = shaderData.VertexCount;

				// vertices
				shaderData.Vertices.resize(shaderData.VertexCount); // TODO: maybe resize in some increments instead?? like 512 or so
				shaderData.VBO = VertexBuffer::Create(sizeof(Vertex) * shaderData.VertexCount);
				shaderData.VBO->SetLayout(Vertex::GetLayout());

				// ids
				shaderData.MaterialID.resize(shaderData.VertexCount);
				shaderData.MaterialIDSSBO = StorageBuffer::Create(sizeof(int) * shaderData.VertexCount);
			}

			// resize index buffer
			if (shaderData.IndexCount > shaderData.CurrentMaxIndexCount)
			{
				shaderData.CurrentMaxIndexCount = shaderData.IndexCount;

				// indices
				shaderData.Indices.resize(shaderData.IndexCount);
				shaderData.IBO = IndexBuffer::Create(shaderData.IndexCount);
			}

			uint32_t vertexOffset = 0;
			uint32_t indexOffset = 0;
			int i = 0;
			for (const auto &meshData : shaderData.Meshes)
			{
				// paste vertices and indices appropriately
				meshData.Mesh->PasteVerticesTransformed(&shaderData.Vertices[vertexOffset], *meshData.Transform.get());
				meshData.Mesh->PasteIndicesOffset(&shaderData.Indices[indexOffset], vertexOffset);

				// set material ids
				//memset(&shaderData.MaterialID[vertexOffset], i++, meshData.Mesh->GetVertices().size());
				for (uint32_t j = 0; j < meshData.Mesh->GetVertices().size(); j++)
					shaderData.MaterialID[vertexOffset + j] = i;
				i++;

				// update offsets
				vertexOffset += meshData.Mesh->GetVertices().size();
				indexOffset += meshData.Mesh->GetIndices().size();
			}

			// update vbo
			shaderData.VBO->SetData(
				shaderData.Vertices.data(),
				sizeof(Vertex) * shaderData.Vertices.size()
			);

			// update ibo
			shaderData.IBO->SetData(
				shaderData.Indices.data(),
				shaderData.Indices.size()
			);

			// update vao
			shaderData.VAO->SetVertexBuffer(shaderData.VBO, 0);
			shaderData.VAO->SetIndexBuffer(shaderData.IBO);


			// upload materials ssbo
			auto materialData = std::vector<PhongMaterial>();
			materialData.resize(shaderData.Materials.size());
			for (int i = 0; i < shaderData.Materials.size(); i++)
				//memcpy_s(&materialData[i], sizeof(PhongMaterial), shaderData.Materials[i].get(), sizeof(PhongMaterial));
				materialData[i] = *shaderData.Materials[i].get();

			if (shaderData.Materials.size() > shaderData.CurrentMaxMaterialCount)
			{
				shaderData.CurrentMaxMaterialCount = shaderData.Materials.size();
				shaderData.MaterialSSBO = StorageBuffer::Create(materialData.data(), sizeof(PhongMaterial) * materialData.size());
				//shaderData.MaterialSSBO->SetData(materialData.data(), materialData.size())
				//shaderData.MaterialSSBO->Bind();
			}

			shaderData.MaterialIDSSBO->SetData(shaderData.MaterialID.data(), sizeof(uint32_t) * shaderData.MaterialID.size());

			shaderData.Shader->Bind();
			shaderData.Shader->SetupStorageBuffer("ss_MaterialIDs", 0, shaderData.MaterialIDSSBO->GetPtr());
			shaderData.Shader->SetupStorageBuffer("ss_Materials", 1, shaderData.MaterialSSBO->GetPtr());

			Flush(shaderData);
		}
	}

	void Renderer::Flush(const PerShaderData &data)
	{
		data.Shader->Bind();
		data.Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.ActiveScene->GetViewProjectionMatrix());
		data.Shader->UploadUniformIntArray("u_Texture", s_TextureSlots, s_TextureSlotsCount);
		data.Shader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f }); // TODO: same thing as with transform; also the case with some other stuff

		data.VAO->Bind();
		data.IBO->Bind();
		data.VBO->Bind();

		RenderCommand::DrawIndexed(data.VAO, data.IndexCount);
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
