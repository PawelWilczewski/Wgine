#pragma once

#include "RenderCommand.h"
#include "Wgine/Camera/Camera.h"
#include "Wgine/Renderer/Shader.h"
#include "Wgine/ECS/Scene.h"

namespace Wgine
{
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

		static void BeginScene(Scene *scene);
		static void Submit(const SceneEntity &entity);
		static void Submit(Ref<Shader> shader, Ref<Mesh> mesh, Ref<glm::mat4> transform);
		static void EndScene();

		static void Flush();

		static API GetAPI() { return s_API; }

		static void OnWindowResized(float width, float height);

	private:
		static API s_API;
	};
}
