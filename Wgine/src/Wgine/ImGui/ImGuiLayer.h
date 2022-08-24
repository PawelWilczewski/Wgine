#pragma once

#include "Wgine/Layer.h"
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

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event &event);

	private:
		bool OnKeyPressed(KeyPressedEvent &e);
		bool OnKeyReleased(KeyReleasedEvent &e);
		bool OnKeyTyped(KeyTypedEvent &e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent &e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent &e);
		bool OnMouseMoved(MouseMovedEvent &e);
		bool OnMouseScrolled(MouseScrolledEvent &e);
		bool OnWindowResized(WindowResizeEvent &e);

	private:
		float m_Time = 0.f;
	};
}

