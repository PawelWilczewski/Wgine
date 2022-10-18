#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Wgine
{
	class Material
	{
	public:
		Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{}

		~Material()
		{}

	public:
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		Ref<Texture2D> DiffuseTex;
		Ref<Texture2D> SpecularTex;
	};

	// TODO: material tiling param for each texture
	struct MaterialGPU
	{
		MaterialGPU(glm::vec3 diffuse = glm::vec3(0.2f, 0.6f, 1.f), glm::vec3 specular = glm::vec3(0.5f), glm::vec3 ambient = glm::vec3(0.2f), int diffuseTex = 0, int specularTex = 0)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{}

		MaterialGPU(const Material &material)
			: Diffuse(material.Diffuse), Specular(material.Specular), Ambient(material.Ambient), DiffuseTex(material.DiffuseTex->GetSlot()), SpecularTex(material.SpecularTex->GetSlot())
		{}

		alignas(16) glm::vec3 Diffuse;
		alignas(16) glm::vec3 Specular;
		alignas(16) glm::vec3 Ambient;

		uint8_t Textures[32];
	};
}
