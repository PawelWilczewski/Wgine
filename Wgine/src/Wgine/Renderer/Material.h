#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Wgine
{
	struct PhongMaterial
	{
		PhongMaterial(glm::vec3 diffuse = glm::vec3(0.2f, 0.6f, 1.f), glm::vec3 specular = glm::vec3(0.5f), glm::vec3 ambient = glm::vec3(0.2f), int diffuseTex = -1, int specularTex = -1)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{
		}

		alignas(16) glm::vec3 Diffuse;
		alignas(16) glm::vec3 Specular;
		alignas(16) glm::vec3 Ambient;
		int32_t DiffuseTex;
		int32_t SpecularTex;
	};

	class Material
	{
	public:
		Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{
		}

		~Material()
		{
		}

	public:
		glm::vec3 Diffuse;
		glm::vec3 Specular; // TODO: is float not sufficient?
		glm::vec3 Ambient; // TODO: is float not sufficient
		Ref<Texture2D> DiffuseTex;
		Ref<Texture2D> SpecularTex;
	};
}
