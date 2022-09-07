#include "WginePCH.h"
#include "OpenGLRenderCommand.h"

#include "glad/glad.h"

namespace Wgine
{
	RenderCommand *RenderCommand::s_Instance = new OpenGLRenderCommand();

	void OpenGLRenderCommand::InitImpl()
	{
		// depth
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_CLAMP);
		glDepthFunc(GL_LEQUAL); // left-handed coordinate system so this is necessary

		// normals
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);

		// antialias
		//glEnable(GL_POLYGON_SMOOTH);
		//glEnable(GL_LINE_SMOOTH);

		// blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderCommand::SetClearColorImpl(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderCommand::SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderCommand::ClearImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderCommand::DrawIndexedImpl(const Ref<VertexArray> &vertexArray, uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count == 0 ? vertexArray->GetIndexBuffer()->GetCount() : count, GL_UNSIGNED_INT, nullptr);
	}
}
