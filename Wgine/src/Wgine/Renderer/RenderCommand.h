#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Wgine
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4 &color) { s_Instance->SetClearColorImpl(color); }
		inline static void Clear() { s_Instance->ClearImpl(); }
		inline static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray) { s_Instance->DrawIndexedImpl(vertexArray); }

	protected:
		virtual void SetClearColorImpl(const glm::vec4 &color) = 0;
		virtual void ClearImpl() = 0;
		virtual void DrawIndexedImpl(const std::shared_ptr<VertexArray> &vertexArray) = 0;

	private:
		static RenderCommand *s_Instance;
	};
}
