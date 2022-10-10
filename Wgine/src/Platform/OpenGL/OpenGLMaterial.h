#pragma once

#include <glm/glm.hpp>
#include "Wgine/Renderer/Material.h"

namespace Wgine
{
	class OpenGLMaterial : public Material
	{
	public:
		OpenGLMaterial(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex)
			: Diffuse(diffuse), Specular(specular), Ambient(ambient), DiffuseTex(diffuseTex), SpecularTex(specularTex)
		{
		}

		~OpenGLMaterial()
		{

		}

	public:
		glm::vec3 Diffuse;
		glm::vec3 Specular;
		glm::vec3 Ambient;
		Ref<Texture2D> DiffuseTex;
		Ref<Texture2D> SpecularTex;
	};
}
