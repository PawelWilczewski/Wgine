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

		for (cgltf_mesh *mesh = data->meshes; mesh < data->meshes + data->meshes_count; mesh++)
		{
			for (cgltf_primitive *primitive = mesh->primitives; primitive < mesh->primitives + mesh->primitives_count; primitive++)
			{
				// append indices
				switch (primitive->indices->component_type)
				{
				case cgltf_component_type_invalid: WGINE_CORE_ASSERT(false, "TODO: fill up");  break;
				case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_r_16u:
				{
					auto test = data->buffers->data;
					// TODO: we should support 16 bit index and more
					auto start = primitive->indices->buffer_view->offset;
					//auto start = primitive->indices->buffer_view->size;

					//auto buffer = data->buffers + (cgltf_buffer *)primitive->indices->buffer_view->buffer->data;
					auto indices = (uint16_t *)primitive->indices->buffer_view->data;
					//AddIndices((uint32_t *)indices, primitive->indices->count);
					break;
				}
				case cgltf_component_type_r_32u: AddIndices((uint32_t *)primitive->indices->buffer_view->data, primitive->indices->count);  break;
				case cgltf_component_type_r_32f: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				case cgltf_component_type_max_enum: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				default: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
				}

				// create the vertices data
				for (cgltf_attribute *attribute = primitive->attributes; attribute < primitive->attributes + primitive->attributes_count; attribute++)
				{
					float *result = new float[attribute->data->count];
					for (int i = 0; i < attribute->data->count; i++)
						cgltf_accessor_read_float(attribute->data, i, &result[i], 3);
					delete[] result;
					switch (attribute->data->type)
					{
					case cgltf_type_invalid: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_scalar: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_vec2: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_vec3: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_vec4: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_mat2: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_mat3: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_mat4: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_type_max_enum: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					default: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}

					switch (attribute->data->component_type)
					{
					case cgltf_component_type_invalid: WGINE_CORE_ASSERT(false, "TODO: fill up");  break;
					case cgltf_component_type_r_8: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_r_8u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_r_16: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_r_16u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_r_32u: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_r_32f: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_component_type_max_enum: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					default: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}

					switch (attribute->type)
					{
					case cgltf_attribute_type_invalid: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_position: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_normal: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_tangent: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_texcoord: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_color: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_joints: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_weights: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_custom: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					case cgltf_attribute_type_max_enum: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					default: WGINE_CORE_ASSERT(false, "TODO: fill up"); break;
					}
				}
			}
		}

		cgltf_free(data);

		// recalc normals if desired
		if (recalculateNormals) RecalculateNormals();
	}

	void Mesh::RecalculateNormals()
	{
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
