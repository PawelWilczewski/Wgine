#include "WginePCH.h"
#include "OpenGLRenderCommand.h"

#include "glad/glad.h"

namespace Wgine
{
	RenderCommand *RenderCommand::s_Instance = new OpenGLRenderCommand();

	void OpenGLRenderCommand::SetClearColorImpl(const glm::vec4 &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderCommand::ClearImpl()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderCommand::DrawIndexedImpl(const Ref<VertexArray> &vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}
