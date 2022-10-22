#pragma once

#include "Wgine/Renderer/RenderCommand.h"

namespace Wgine
{
	class OpenGLRenderCommand : public RenderCommand
	{
	public:
		virtual void InitImpl() override;
		virtual void SetClearColorImpl(const glm::vec4 &color) override;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void ClearImpl() override;

		virtual void DrawIndexedImpl(const Ref<VertexArray> &vertexArray, uint32_t count) override;
		virtual void DrawLinesImpl(uint32_t count, uint32_t offsetCount) override;
	};
}
