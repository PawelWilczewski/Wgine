#pragma once

#include "RenderCommand.h"
#include "Wgine/Camera.h"
#include "Wgine/Renderer/Shader.h"
#include "Wgine/ECS/Scene.h"

namespace Wgine
{
	// TODO: this looks much like a singelton which it shouldn't be: why can we only have one renderer?
	class Renderer
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};

	public:
		static void Init();

		static void BeginScene(Scene *scene);
		static void EndScene();

		static void Submit(const SceneEntity &entity);
		static void Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform);

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;

	private:
		static Scene *m_ActiveScene;
	};
}
