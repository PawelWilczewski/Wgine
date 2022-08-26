#pragma once

#include "Wgine/Renderer/RenderCommand.h"

namespace Wgine
{
	class OpenGLRenderCommand : public RenderCommand
	{
	public:
		virtual void SetClearColorImpl(const glm::vec4 &color) override;
		virtual void ClearImpl() override;

		virtual void DrawIndexedImpl(const std::shared_ptr<VertexArray> &vertexArray) override;
	};
}
