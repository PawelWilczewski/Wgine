#pragma once

#include "Core.h"

#include "Window.h"
#include "Wgine/LayerStack.h"
#include "Event/ApplicationEvent.h"

namespace Wgine {
	class WGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *overlay);

	private:
		bool OnWindowClosed(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// to be defined in CLIENT
	Application *CreateApplication();
}
