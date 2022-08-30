#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Wgine
{
	class RenderCommand
	{
	public:
		inline static void Init() { s_Instance->InitImpl(); };
		inline static void SetClearColor(const glm::vec4 &color) { s_Instance->SetClearColorImpl(color); }
		inline static void Clear() { s_Instance->ClearImpl(); }
		inline static void DrawIndexed(const Ref<VertexArray> &vertexArray) { s_Instance->DrawIndexedImpl(vertexArray); }

	protected:
		virtual void InitImpl() = 0;
		virtual void SetClearColorImpl(const glm::vec4 &color) = 0;
		virtual void ClearImpl() = 0;
		virtual void DrawIndexedImpl(const Ref<VertexArray> &vertexArray) = 0;

	private:
		static RenderCommand *s_Instance;
	};
}
