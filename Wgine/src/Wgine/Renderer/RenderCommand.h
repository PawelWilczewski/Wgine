#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Wgine
{
	// TODO: maybe merge with renderer?
	class RenderCommand
	{
	public:
		inline static void Init() { s_Instance->InitImpl(); };
		inline static void SetClearColor(const glm::vec4 &color) { s_Instance->SetClearColorImpl(color); }
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_Instance->SetViewportImpl(x, y, width, height); }
		inline static void Clear() { s_Instance->ClearImpl(); }
		inline static void DrawIndexed(const Ref<VertexArray> &vertexArray) { s_Instance->DrawIndexedImpl(vertexArray); }

	protected:
		virtual void InitImpl() = 0;
		virtual void SetClearColorImpl(const glm::vec4 &color) = 0;
		virtual void SetViewportImpl(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearImpl() = 0;
		virtual void DrawIndexedImpl(const Ref<VertexArray> &vertexArray) = 0;

	private:
		static RenderCommand *s_Instance;
	};
}
