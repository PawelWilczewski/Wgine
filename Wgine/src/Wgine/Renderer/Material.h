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
