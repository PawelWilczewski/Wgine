#include "WginePCH.h"
#include "Mesh.h"
#include "Wgine/Renderer/RendererDebug.h"
#include "cgltf.h"


namespace Wgine
{
	static float angleBetweenVectors(const glm::vec3 &v1, const glm::vec3 &v2) { return glm::acos(glm::dot(glm::normalize(v1), (glm::normalize(v2)))); }

	Mesh::Mesh(const std::string &filePath, bool recalculateNormals)
	{
		// determine the file extension
		auto p = std::filesystem::absolute(filePath);
		auto fullPath = p.string();
		const std::string extension = p.extension().string();
		cgltf_file_type filetype = cgltf_file_type_invalid;
		if (extension == ".gltf")
			filetype = cgltf_file_type_gltf;
		else if (extension == ".glb")
			filetype = cgltf_file_type_glb;
		else
			WGINE_CORE_ASSERT(false, "Unknown extension for importing mesh {0}", filePath);

		// set the import options
		cgltf_options options = { filetype };
		cgltf_data *data = nullptr;
		
		// parse
		cgltf_result result = cgltf_parse_file(&options, fullPath.c_str(), &data);
		WGINE_CORE_ASSERT(result == cgltf_result_success, "Error importing a mesh from .gltf file: {0}", fullPath);
		cgltf_load_buffers(&options, data, fullPath.c_str());

		bool uvImported = false;
		for (cgltf_mesh *mesh = data->meshes; mesh < data->meshes + data->meshes_count; mesh++)
		{
			for (cgltf_primitive *primitive = mesh->primitives; primitive < mesh->primitives + mesh->primitives_count; primitive++)
			{
				// append indices
				switch (primitive->indices->component_type)
				{
				case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_16u:
				{
					auto indices = (uint16_t *)cgltf_buffer_view_data(primitive->indices->buffer_view);
					auto indices32 = MakeScope<uint32_t[]>(primitive->indices->count);
					for (int i = 0; i < primitive->indices->count; i++)
						indices32[i] = (uint32_t)indices[i];
					AddIndices(indices32.get(), primitive->indices->count);

					// reverse order because we're using front face culling
					std::reverse(m_Indices.begin(), m_Indices.end());

					break;
				}
				case cgltf_component_type_r_32u:
				{
					AddIndices((uint32_t *)cgltf_buffer_view_data(primitive->indices->buffer_view), primitive->indices->count);
					break;
				}
				case cgltf_component_type_r_32f: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_max_enum: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				default: WGINE_CORE_ASSERT(false, "Invalid index component type for imported mesh!"); break;
				}

				// parse the vertices data
				for (cgltf_attribute *attribute = primitive->attributes; attribute < primitive->attributes + primitive->attributes_count; attribute++)
				{
					switch (attribute->type)
					{
					case cgltf_attribute_type_position:
					{
						switch (attribute->data->type)
						{
						case cgltf_type_vec2: WGINE_CORE_ASSERT(false, "Mesh position data as 2D vector!"); break;
						case cgltf_type_vec3:
						{
							switch (attribute->data->component_type)
							{
							case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32f:
							{
								auto bufferData = (glm::vec3 *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].Position = bufferData[i];
								break;
							}
							default: WGINE_CORE_ASSERT(false, "Invalid component type for position of imported mesh!"); break;
							}

							break;
						}
						case cgltf_type_vec4: WGINE_CORE_ASSERT(false, "Mesh position data as 4D vector!"); break;
						default: WGINE_CORE_ASSERT(false, "Invalid position data type for imported mesh!"); break;
						}
						break;
					}
					case cgltf_attribute_type_normal:
					{
						switch (attribute->data->type)
						{
						case cgltf_type_vec2: WGINE_CORE_ASSERT(false, "Mesh normal data as 2D vector!"); break;
						case cgltf_type_vec3:
						{
							switch (attribute->data->component_type)
							{
							case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32f:
							{
								auto bufferData = (glm::vec3 *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].Normal = bufferData[i];
								break;
							}
							default: WGINE_CORE_ASSERT(false, "Invalid component type for normal of imported mesh!"); break;
							}
							break;
						}
						case cgltf_type_vec4: WGINE_CORE_ASSERT(false, "Mesh normal data as 4D vector!"); break;
						default: WGINE_CORE_ASSERT(false, "Invalid normal data type for imported mesh!"); break;
						}
						break;
					}
					case cgltf_attribute_type_tangent:
					{
						WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
						break;
					}
					case cgltf_attribute_type_texcoord:
					{
						// only import the first uv coords
						if (uvImported) break;

						switch (attribute->data->type)
						{
						case cgltf_type_vec2: 
						{
							switch (attribute->data->component_type)
							{
							case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32f:
							{
								auto bufferData = (glm::vec2 *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].TexCoord = bufferData[i];
								uvImported = true;
								break;
							}
							default: WGINE_CORE_ASSERT(false, "Invalid component type for texcoord of imported mesh!"); break;
							}
							break;
						}
						case cgltf_type_vec3: WGINE_CORE_ASSERT(false, "Mesh texcoords data as 3D vector!"); break;
						case cgltf_type_vec4: WGINE_CORE_ASSERT(false, "Mesh texcoords data as 4D vector!"); break;
						default: WGINE_CORE_ASSERT(false, "Invalid texcoords data type for imported mesh!"); break;
						}
						break;
					}
					case cgltf_attribute_type_color:
					{
						switch (attribute->data->type)
						{
						case cgltf_type_vec2: WGINE_CORE_ASSERT(false, "Mesh color data as 2D vector!"); break;
						case cgltf_type_vec3:
						{
							switch (attribute->data->component_type)
							{
							case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32f:
							{
								auto bufferData = (glm::vec3 *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].Color = bufferData[i];
								break;
							}
							default: WGINE_CORE_ASSERT(false, "Invalid component type for color of imported mesh!"); break;
							}
							break;
						}
						case cgltf_type_vec4:
						{
							switch (attribute->data->component_type)
							{
							case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_16u:
							{
								auto bufferData = (glm::vec<4, uint16_t> *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].Color = glm::vec3(bufferData[i].x / 255.f, bufferData[i].y / 255.f, bufferData[i].z/ 255.f);
								break;
							}
							case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
							case cgltf_component_type_r_32f:
							{
								auto bufferData = (glm::vec4 *)cgltf_buffer_view_data(attribute->data->buffer_view);
								if (m_Vertices.size() < attribute->data->count)
									m_Vertices.resize(attribute->data->count);
								for (int i = 0; i < attribute->data->count; i++)
									m_Vertices[i].Color = bufferData[i];
								break;
							}
							default: WGINE_CORE_ASSERT(false, "Invalid component type for color of imported mesh!"); break;
							}
							break;
						}
						default: WGINE_CORE_ASSERT(false, "Invalid color data type for imported mesh!"); break;
						}
						break;
					}
					case cgltf_attribute_type_joints:
					{
						WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}
					case cgltf_attribute_type_weights:
					{
						WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}
					case cgltf_attribute_type_custom:
					{
						WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}
					case cgltf_attribute_type_max_enum:
					{
						WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}
					default:
					{
						WGINE_CORE_ASSERT(false, "Invalid/not implemented attribute type for imported mesh!"); break; // TODO: probs just skip instead
					}
					}
				}
			}
		}

		cgltf_free(data);

		// recalc normals if desired
		if (recalculateNormals) RecalculateNormals();
	}

	void Mesh::AddVertex(const Vertex &v)
	{
		m_DirtyVertices = true;
		m_Vertices.push_back(v);
	}

	void Mesh::AddVertices(std::vector<Vertex> v)
	{
		m_DirtyVertices = true;
		m_Vertices.insert(m_Vertices.end(), v.begin(), v.end());
	}

	void Mesh::AddVertices(Vertex *v, int count)
	{
		m_DirtyVertices = true;
		m_Vertices.insert(m_Vertices.end(), v, v + count);
	}

	void Mesh::AddIndex(uint32_t i)
	{
		m_DirtyIndices = true;
		m_Indices.push_back(i);
	}

	void Mesh::AddIndices(uint32_t *i, int count)
	{
		m_DirtyIndices = true;
		m_Indices.insert(m_Indices.end(), i, i + count);
	}

	void Mesh::AddIndices(std::vector<uint32_t> i)
	{
		m_DirtyIndices = true;
		m_Indices.insert(m_Indices.end(), i.begin(), i.end());
	}

	void Mesh::AddTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2)
	{
		m_DirtyIndices = true;
		m_DirtyVertices = true;
		uint32_t nextIndex = m_Vertices.size();
		m_Vertices.insert(m_Vertices.end(), { v0, v1, v2 });
		m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2 });
	}

	void Mesh::AddQuad(const Vertex &v0, const Vertex &v1, Vertex v2, Vertex v3)
	{
		m_DirtyIndices = true;
		m_DirtyVertices = true;
		uint32_t nextIndex = m_Vertices.size();
		m_Vertices.insert(m_Vertices.end(), { v0, v1, v2, v3 });
		m_Indices.insert(m_Indices.end(), { nextIndex, nextIndex + 1, nextIndex + 2, nextIndex + 2, nextIndex + 3, nextIndex });
	}

	void Mesh::RecalculateNormals()
	{
		m_DirtyVertices = true;

		// area and angle-weighted smooth shading
		for (int i = 0; i < m_Indices.size(); i += 3)
		{
			auto v10 = m_Vertices[m_Indices[i + 1]].Position - m_Vertices[m_Indices[i]].Position;
			auto v20 = m_Vertices[m_Indices[i + 2]].Position - m_Vertices[m_Indices[i]].Position;
			auto v21 = m_Vertices[m_Indices[i + 2]].Position - m_Vertices[m_Indices[i + 1]].Position;
			auto cross = glm::cross(v20, v10);
			auto normal = glm::normalize(cross);
			auto area = 0.5f * glm::length(cross);
			auto angle0 = angleBetweenVectors(v10, v20);
			auto angle1 = angleBetweenVectors(v21, -v10);
			auto angle2 = angleBetweenVectors(-v20, -v21);
				
			m_Vertices[m_Indices[i]].Normal += normal * area * angle0;
			m_Vertices[m_Indices[i + 1]].Normal += normal * area * angle1;
			m_Vertices[m_Indices[i + 2]].Normal += normal * area * angle2;
		}

		for (auto &vert : m_Vertices)
			vert.Normal = glm::normalize(vert.Normal);
	}
}
