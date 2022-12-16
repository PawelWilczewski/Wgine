#pragma once

#include <glm/glm.hpp>

#include "Texture.h"
#include "TextureLibrary.h"

namespace Wgine
{
	class Material
	{
	public:
		Material(glm::vec3 diffuse = glm::vec3(0.2f, 0.6f, 1.f), float specular = 0.5f, float ambient = 1.0f, float shininess = 32.f)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), Shininess(shininess), DiffuseTex(nullptr), SpecularTex(nullptr)
		{}

		Material(glm::vec3 diffuse, float specular, float ambient, float shininess, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), Shininess(shininess), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{}

		Material(glm::vec3 diffuse, float specular, float ambient, float shininess, Ref<Texture2D> diffuseTex)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), Shininess(shininess), DiffuseTex(diffuseTex), SpecularTex(nullptr)
		{}

		~Material()
		{}

	public:
		glm::vec3 Diffuse;
		float Specular;
		float Ambient; // ambient * diffuse for ambience surface color
		float Shininess;
		Ref<Texture2D> DiffuseTex; // TODO: dynamic texture count
		Ref<Texture2D> SpecularTex; // TODO: dynamic texture count
	};

	// TODO: material tiling param for each texture
	struct MaterialGPU
	{
		MaterialGPU(glm::vec3 diffuse = glm::vec3(0.2f, 0.6f, 1.f), float specular = 0.5f, float ambient = 1.0f, float shininess = 0.5f)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), Shininess(shininess)
		{}

		MaterialGPU(const Material &material)
			: Diffuse(material.Diffuse), Specular(material.Specular), Ambient(material.Ambient), Shininess(material.Shininess)
		{}

		alignas(16) glm::vec3 Diffuse;
		float Specular;
		float Ambient;
		float Shininess;
		uint8_t Textures[32];
	};
}
