#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Wgine
{
	class RenderCommand
	{
	public:
		static void Init() { s_Instance->InitImpl(); };
		static void SetClearColor(const glm::vec4 &color) { s_Instance->SetClearColorImpl(color); }
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_Instance->SetViewportImpl(x, y, width, height); }
		static void Clear() { s_Instance->ClearImpl(); }
		static void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t count = 0) { s_Instance->DrawIndexedImpl(vertexArray, count); }
		static void DrawLines(uint32_t count, uint32_t offsetCount = 0U) { s_Instance->DrawLinesImpl(count, offsetCount); }

	protected:
		virtual void InitImpl() = 0;
		virtual void SetClearColorImpl(const glm::vec4 &color) = 0;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearImpl() = 0;
		virtual void DrawIndexedImpl(const Ref<VertexArray> &vertexArray, uint32_t count) = 0;
		virtual void DrawLinesImpl(uint32_t count, uint32_t offsetCount) = 0;

	private:
		static RenderCommand *s_Instance;
	};
}
