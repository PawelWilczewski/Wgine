#pragma once

#include "RenderCommand.h"
#include "Wgine/Camera.h"
#include "Wgine/Renderer/Shader.h"

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
		static void BeginScene(Camera &camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray);

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData *m_SceneData;
	};
}
