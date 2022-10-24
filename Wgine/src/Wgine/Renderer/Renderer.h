#pragma once

#include "RenderCommand.h"
#include "Wgine/Camera/Camera.h"
#include "Wgine/Renderer/Shader.h"
#include "Wgine/ECS/Scene.h"
#include "Wgine/Renderer/Light.h"

namespace Wgine
{
	struct PerShaderData;

	class Renderer
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		static void Init();
		static void Shutdown();

		static void SetActiveCamera(Camera *camera);

		static void Submit(Scene *scene);
		static void Submit(const SceneEntity &entity); // TODO: pointer here to be more consistent
		static void Submit(Ref<Shader> shader, Ref<Material> material, Ref<Mesh> mesh, const Transform &transform);
		static void Submit(Light *light);
		static void Flush();

		static API GetAPI() { return s_API; }

		static void OnWindowResized(float width, float height);

		static int s_TextureSlots[];
		static const uint32_t s_TextureSlotsCount;

	private:
		static API s_API;
	};
}
