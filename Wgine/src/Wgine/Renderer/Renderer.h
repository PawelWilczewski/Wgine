#pragma once

#include "RenderCommand.h"

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
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray> &vertexArray);

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
