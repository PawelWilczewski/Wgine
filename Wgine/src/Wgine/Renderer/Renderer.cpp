#include "WginePCH.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Vertex.h"

#include "Wgine/Core/Core.h"
#include "Wgine/Core/Time.h"
#include "Wgine/Renderer/Mesh.h"
#include "Wgine/Renderer/Vertex.h"
#include "Wgine/Renderer/Material.h"
#include "Wgine/Renderer/Light.h"
#include "Wgine/Renderer/RendererDebug.h"

namespace Wgine
{
	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	const uint32_t Renderer::s_TextureSlotsCount = 32;
	int Renderer::s_TextureSlots[s_TextureSlotsCount] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	
	// invalid texture is uint8(-1) = 255
	constexpr uint8_t INVALID_TEXTURE_SLOT = -1;

	class RendererData
	{
	public:
		RendererData()
			: m_TextureSlot(0),
			m_AmbientLights({}),
			m_DirectionalLights({}),
			m_PointLights({}),
			m_SpotLights({})
		{
		}

		void Init()
		{
			AmbientLightsSSBO = StorageBuffer::Create(sizeof(AmbientLightGPU) * 1);
			DirectionalLightsSSBO = StorageBuffer::Create(sizeof(DirectionalLightGPU) * 1);
			PointLightsSSBO = StorageBuffer::Create(sizeof(PointLightGPU) * 16);
			SpotLightsSSBO = StorageBuffer::Create(sizeof(SpotLightGPU) * 8);
		}

		// returns texture slot beginning at 0
		uint8_t BindTexture(Ref<Texture2D> texture)
		{
			auto slot = FindTextureSlot(texture);
			if (slot == -1) // texture is not bound yet
			{
				m_Textures[m_TextureSlot++] = texture;
				texture->Bind(m_TextureSlot - 1);
				return m_TextureSlot - 1;
			}
			else // texture already bound
				return slot;
		}

		void ResetTextures()
		{
			m_TextureSlot = 0;
		}

		uint32_t GetFreeSlotsCount() const { return 32 - m_TextureSlot; }

		void ResetLights()
		{
			m_AmbientLights.clear();
			m_DirectionalLights.clear();
			m_PointLights.clear();
			m_SpotLights.clear();
		}

		void AddAmbientLight(AmbientLight *l) { m_AmbientLights.push_back(*l); }
		void AddDirectionalLight(DirectionalLight *l) { m_DirectionalLights.push_back(*l); }
		void AddPointLight(PointLight *l) { m_PointLights.push_back(*l); }
		void AddSpotLight(SpotLight *l) { m_SpotLights.push_back(*l); }
		void UploadLights()
		{
			AmbientLightsSSBO->SetData(m_AmbientLights.data(), sizeof(AmbientLightGPU) * m_AmbientLights.size());
			DirectionalLightsSSBO->SetData(m_DirectionalLights.data(), sizeof(DirectionalLightGPU) * m_DirectionalLights.size());
			PointLightsSSBO->SetData(m_PointLights.data(), sizeof(PointLightGPU) * m_PointLights.size());
			SpotLightsSSBO->SetData(m_SpotLights.data(), sizeof(SpotLightGPU) * m_SpotLights.size());
		}

	private:
		// returns unsigned -1 (largest uint32) in case texture is not bound
		uint32_t FindTextureSlot(Ref<Texture2D> texture)
		{
			auto it = std::find(m_Textures.begin(), m_Textures.end(), texture);
			return it == m_Textures.end() ? -1 : it - m_Textures.begin();
		}

	public:
		Camera *ActiveCamera = nullptr;
		Ref<StorageBuffer> AmbientLightsSSBO;
		Ref<StorageBuffer> DirectionalLightsSSBO;
		Ref<StorageBuffer> PointLightsSSBO;
		Ref<StorageBuffer> SpotLightsSSBO;

	private:
		uint32_t m_TextureSlot;
		std::array<Ref<Texture2D>, Renderer::s_TextureSlotsCount> m_Textures;

		std::vector<AmbientLightGPU> m_AmbientLights;
		std::vector<DirectionalLightGPU> m_DirectionalLights;
		std::vector<PointLightGPU> m_PointLights;
		std::vector<SpotLightGPU> m_SpotLights;
	};

	static RendererData s_RendererData = RendererData();

	struct PerShaderData
	{
		PerShaderData()
		{}

		PerShaderData(Ref<Shader> s)
			: Shader(s), VAO(VertexArray::Create())
		{
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
			MaterialSSBO = StorageBuffer::Create(sizeof(MaterialGPU) * 100);
			MaterialIDSSBO = StorageBuffer::Create(sizeof(int32_t) * 500000);

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
			VAO->Bind();
			VBO->Bind();
			IBO->Bind();

			Shader->Bind(); // TODO: URGENT, instead of always doing this, each shader should have a function responsible for this so we don't send unnecessary data/data to unbound ssbos
			Shader->SetupStorageBuffer("ss_MaterialIDs", 0, MaterialIDSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_Materials", 1, MaterialSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_TransformIDs", 2, TransformIDSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_Transforms", 3, TransformSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_AmbientLights", 4, s_RendererData.AmbientLightsSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_DirectionalLights", 5, s_RendererData.DirectionalLightsSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_PointLights", 6, s_RendererData.PointLightsSSBO->GetPtr());
			Shader->SetupStorageBuffer("ss_SpotLights", 7, s_RendererData.SpotLightsSSBO->GetPtr());
			// TODO: use ubo for these 3 (at least for sure for u_Texture to avoid sending all that each time)
			Shader->UploadUniformMat4("u_ViewProjection", s_RendererData.ActiveCamera->GetViewProjectionMatrix());
			Shader->UploadUniformFloat3("u_CameraLocation", s_RendererData.ActiveCamera->GetLocation());
			// TODO: can this be uploaded only once at the start?
			Shader->UploadUniformIntArray("u_Texture", Renderer::s_TextureSlots, Renderer::s_TextureSlotsCount);

			VBO->SetData(Vertices.data(), sizeof(Vertex) * Vertices.size());
			IBO->SetData(Indices.data(), Indices.size());
			TransformSSBO->SetData(Transforms.data(), sizeof(TransformGPU) * Transforms.size());
			TransformIDSSBO->SetData(TransformIDs.data(), sizeof(int32_t) * TransformIDs.size());

			Scope<MaterialGPU[]> materialsData(new MaterialGPU[Materials.size()]);
			for (int i = 0; i < Materials.size(); i++)
			{
				if (s_RendererData.GetFreeSlotsCount() < 2) // TODO: adjust to include dynamic texture count
				{
					// TODO: flush and reset textures (probably we should not reset the vbo, ibo etc. but render it partially)
					WGINE_CORE_ASSERT(false, "TODO: Implement flushing when exceeded texture slots limit!");
				}
				else
				{
					materialsData[i] = *Materials[i].get();
					materialsData[i].Textures[0] = Materials[i]->DiffuseTex ? s_RendererData.BindTexture(Materials[i]->DiffuseTex) : INVALID_TEXTURE_SLOT;
					materialsData[i].Textures[1] = Materials[i]->SpecularTex ? s_RendererData.BindTexture(Materials[i]->SpecularTex) : INVALID_TEXTURE_SLOT;
				}
			}

			MaterialSSBO->SetData(materialsData.get(), sizeof(MaterialGPU) * Materials.size());
			MaterialIDSSBO->SetData(MaterialIDs.data(), sizeof(int32_t) * MaterialIDs.size());

			RenderCommand::DrawIndexed(VAO, Indices.size());

			Reset();
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
		std::vector<Ref<Material>> Materials;
		std::vector<int32_t> MaterialIDs;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	static std::unordered_map<std::string, PerShaderData> s_ShaderData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		RendererDebug::Init();

		s_ShaderData = std::unordered_map<std::string, PerShaderData>();
		s_RendererData.Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::SetActiveCamera(Camera *camera)
	{
		s_RendererData.ActiveCamera = camera;
		RendererDebug::SetActiveCamera(camera);
	}

	void Renderer::Submit(Scene *scene)
	{
		WGINE_CORE_ASSERT(scene, "Invalid scene for renderer!");

		for (auto entity : scene->m_SceneEntities)
			Submit(*entity);

		for (auto light : scene->m_Lights)
			Submit(light);
	}

	void Renderer::Submit(const SceneEntity &entity)
	{
		Submit(entity.ShaderData, entity.MaterialData, entity.MeshData, entity.GetTransform());
	}

	// TODO: no need to keep references? just add to the resultant array that will be sent to the GPU (copying is necessary no matter what?)
	void Renderer::Submit(Ref<Shader> shader, Ref<Material> material, Ref<Mesh> mesh, const Transform &transform)
	{
		WGINE_ASSERT(s_RendererData.ActiveCamera, "No active camera for renderer!");

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

		// material
		uint32_t index;
		auto findMaterial = std::find(shaderData.Materials.begin(), shaderData.Materials.end(), material);
		if (findMaterial != shaderData.Materials.end())
			index = findMaterial - shaderData.Materials.begin();
		else
		{
			index = shaderData.Materials.size();
			shaderData.Materials.push_back(material);
		}

		// material id
		for (int i = 0; i < mesh->GetVertices().size(); i++)
			shaderData.MaterialIDs.push_back(index);
	}

	void Renderer::Submit(Light *light)
	{
		switch (light->GetLightType())
		{
		case Light::Type::Ambient:
		{
			s_RendererData.AddAmbientLight(static_cast<AmbientLight *>(light));
			break;
		}
		case Light::Type::Directional:
		{
			s_RendererData.AddDirectionalLight(static_cast<DirectionalLight *>(light));
			break;
		}
		case Light::Type::Point:
		{
			s_RendererData.AddPointLight(static_cast<PointLight *>(light));
			break;
		}
		case Light::Type::Spot:
		{
			s_RendererData.AddSpotLight(static_cast<SpotLight *>(light));
			break;
		}
		default:
			WGINE_CORE_ASSERT(false, "Invalid light type / not implemented light type!");
			break;
		}
	}

	void Renderer::Flush()
	{
		s_RendererData.UploadLights();

		for (auto &[shaderName, shaderData] : s_ShaderData)
			shaderData.Flush();

		RendererDebug::Flush();

		s_RendererData.ResetLights();
	}

	void Renderer::OnWindowResized(float width, float height)
	{
		RenderCommand::SetViewport(0, 0, (uint32_t)width, (uint32_t)height);
	}
}
