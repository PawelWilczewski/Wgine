#include "WginePCH.h"
#include "Material.h"

#include "Wgine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLMaterial.h"

namespace Wgine
{
	Ref<Material> Material::Create(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, Ref<Texture2D> diffuseTex, Ref<Texture2D> specularTex)
	{
		SWITCH_RENDERAPI_MAKEREF(Material, diffuse, specular, ambient, diffuseTex, specularTex);
	}
}
