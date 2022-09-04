#pragma once

#include "Wgine/Renderer/Texture.h"
#include "Wgine/ECS/Scene.h"

#include <glm/glm.hpp>

namespace Wgine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Scene *scene);
		static void EndScene();

		// +x to the right (+y in 3d space)
		// +y upwards (+z in 3d space)
		static void DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const glm::vec4 &color);

		// +x to the right (+y in 3d space)
		// +y upwards (+z in 3d space)
		static void DrawQuad(const glm::vec2 &location, float rotation, const glm::vec2 &scale, const Texture2D &texture);

		// in 3d space
		static void DrawQuad(const Transform &transform, const glm::vec4 &color);

		// in 3d space
		static void DrawQuad(const Transform &transform, const Texture2D &texture); // TODO: tint (* tint in .glsl)
	};
}
