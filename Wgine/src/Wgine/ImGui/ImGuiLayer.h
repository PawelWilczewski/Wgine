#pragma once

#include "Wgine/Core/Layer.h"
#include "Wgine/Event/KeyEvent.h"
#include "Wgine/Event/MouseEvent.h"
#include "Wgine/Event/ApplicationEvent.h"

namespace Wgine
{
	class WGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.f;
	};
}
