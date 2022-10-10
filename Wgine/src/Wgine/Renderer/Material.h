#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Wgine
{
	class Material
	{
	public:
		virtual ~Material();

		Ref<Material> Create(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex);
	};
}
