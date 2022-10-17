#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Vertex.h"

#include "Wgine/Core/Core.h"
#include "Wgine/Core/Time.h"
#include "Wgine/Renderer/Mesh.h"
#include "Wgine/Renderer/Vertex.h"
#include "Wgine/Renderer/Material.h"

#include "TextureLibrary.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;

	const uint32_t Renderer::s_TextureSlotsCount = 32;
	int Renderer::s_TextureSlots[s_TextureSlotsCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };

	constexpr uint32_t MaterialsBindingSlot = 0;
	
	struct RendererData
	{
		Scene *ActiveScene = nullptr;
	};

	static RendererData s_RendererData = RendererData();

	struct PerShaderData
	{
		PerShaderData()
		{}

		PerShaderData(Ref<Shader> s)
			: Shader(s), VAO(VertexArray::Create())
		{
			auto timeStart = Time::GetTimeSeconds();
			Vertices.reserve(500000);
			Indices.reserve(500000);
			Materials.reserve(500);
			MaterialIDs.reserve(500000);
			Transforms.reserve(5000);
			TransformIDs.reserve(500000);
			Meshes.reserve(5000);

			VAO = VertexArray::Create();
			VBO = VertexBuffer::Create(sizeof(Vertex) * 500000);
			VBO->SetLayout(Vertex::GetLayout());
			IBO = IndexBuffer::Create(500000);
			VAO->AddVertexBuffer(VBO);
			VAO->SetIndexBuffer(IBO);

			TransformSSBO = StorageBuffer::Create(sizeof(TransformGPU) * 5000);
			TransformIDSSBO = StorageBuffer::Create(sizeof(int32_t) * 500000);
			MaterialSSBO = StorageBuffer::Create(sizeof(PhongMaterial) * 100);
			MaterialIDSSBO = StorageBuffer::Create(sizeof(uint32_t) * 500000);

			Reset();
		}

		void Reset()
		{
			Meshes.clear();
			Transforms.clear();
			TransformIDs.clear();
			Materials.clear();
			MaterialIDs.clear();
			Vertices.clear();
			Indices.clear();
		}

		void Flush()
		{
			VBO->SetData(Vertices.data(), sizeof(Vertex) * Vertices.size());
			IBO->SetData(Indices.data(), Indices.size());
			TransformSSBO->SetData(Transforms.data(), sizeof(TransformGPU) * Transforms.size());
			TransformIDSSBO->SetData(TransformIDs.data(), sizeof(int32_t) * TransformIDs.size());
			Scope<PhongMaterial[]> materialsData(new PhongMaterial[Materials.size()]);
			for (int i = 0; i < Materials.size(); i++)
				materialsData[i] = *Materials[i].get();
			MaterialSSBO->SetData(materialsData.get(), sizeof(PhongMaterial) * Materials.size());
			MaterialIDSSBO->SetData(MaterialIDs.data(), sizeof(int32_t) * MaterialIDs.size());

			Shader->Bind();
			TextureLibrary::Get("assets/textures/coords.png")->Bind(0);
			TextureLibrary::Get("assets/textures/transparent.png")->Bind(1);

			Shader->SetupStorageBuffer("ss_MaterialIDs", 0, MaterialIDSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_Materials", 1, MaterialSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_TransformIDs", 2, TransformIDSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_Transforms", 3, TransformSSBO->GetPtr());

			Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.ActiveScene->GetViewProjectionMatrix());
			Shader->UploadUniformIntArray("u_Texture", Renderer::s_TextureSlots, Renderer::s_TextureSlotsCount);
			//Shader->UploadUniformFloat2("u_Tiling", { 1.f, 1.f }); // TODO: material tiling param for each texture

			VAO->Bind();
			IBO->Bind();
			VBO->Bind();

			RenderCommand::DrawIndexed(VAO, Indices.size());
		}

		Ref<Shader> Shader;
		std::vector<Ref<Mesh>> Meshes;

		Ref<VertexArray> VAO;
		Ref<VertexBuffer> VBO;
		Ref<IndexBuffer> IBO;
		Ref<StorageBuffer> TransformSSBO;
		Ref<StorageBuffer> TransformIDSSBO;
		Ref<StorageBuffer> MaterialSSBO;
		Ref<StorageBuffer> MaterialIDSSBO;

		std::vector<TransformGPU> Transforms;
		std::vector<int32_t> TransformIDs;
		std::vector<Ref<PhongMaterial>> Materials;
		std::vector<int32_t> MaterialIDs;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	static std::unordered_map<std::string, PerShaderData> s_ShaderData;

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
			shaderData.Reset();

		for (auto entity : s_RendererData.ActiveScene->m_SceneEntities)
			Submit(*entity);
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Renderer::Submit(entity.ShaderData, entity.MaterialData, entity.MeshData, entity.GetTransform());
	}

	// TODO: no need to keep references? just add to the resultant array that will be sent to the GPU (copying is necessary no matter what?)
	void Renderer::Submit(Ref<Shader> shader, Ref<PhongMaterial> material, Ref<Mesh> mesh, const Transform &transform)
	{
		WGINE_ASSERT(s_RendererData.ActiveScene, "No active scene for renderer!");

		// TODO: in order to greatly improve performance, only update "dirty" entities (use some array mask to make it more efficient)

		if (!mesh)
			return;

		// new Shader
		if (s_ShaderData.find(shader->GetPath()) == s_ShaderData.end()) // TODO: when switched c++ 20 use .contains instead
			s_ShaderData[shader->GetPath()] = PerShaderData(shader);

		auto &shaderData = s_ShaderData[shader->GetPath()];

		// push meshes, transforms
		shaderData.Meshes.push_back(mesh);
		int32_t transformID = shaderData.Transforms.size();
		shaderData.Transforms.push_back(transform);

		// push transform ids
		for (int i = 0; i < mesh->GetVertices().size(); i++)
			shaderData.TransformIDs.push_back(transformID);

		// push offset indices
		auto offset = shaderData.Vertices.size();
		auto &indices = mesh->GetIndices();
		for (int i = 0; i < indices.size(); i++)
			shaderData.Indices.push_back(indices[i] + offset);

		// push vertices
		for (int i = 0; i < mesh->GetVertices().size(); i++)
			shaderData.Vertices.push_back(mesh->GetVertices()[i]);
		//shaderData.Vertices.insert(shaderData.Vertices.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());
		
		// push back materials and material ids
		uint32_t index;
		auto findMaterial = std::find(shaderData.Materials.begin(), shaderData.Materials.end(), material);
		if (findMaterial != shaderData.Materials.end())
			index = findMaterial - shaderData.Materials.begin();
		else
		{
			index = shaderData.Materials.size();
			shaderData.Materials.push_back(material);
		}
		for (int i = 0; i < mesh->GetVertices().size(); i++)
			shaderData.MaterialIDs.push_back(index);		
	}

	void Renderer::EndScene()
	{
		for (auto &[shaderName, shaderData] : s_ShaderData)
			Flush(shaderData);
	}

	void Renderer::Flush(PerShaderData &data)
	{
		data.Flush();
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
