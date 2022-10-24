#pragma once

#include "Wgine/Camera/Camera.h"

#include <glm/glm.hpp>

namespace Wgine
{
	// TODO: exclude in dist builds
	class RendererDebug
	{
	public:
		static void Init();
		static void Flush();
		static void SetActiveCamera(Camera *camera);

		static void DrawLine(const glm::vec3 &from, const glm::vec3 &to, const glm::vec3 &color = glm::vec3(1.f));
	};
}
