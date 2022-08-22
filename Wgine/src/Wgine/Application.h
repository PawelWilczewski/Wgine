#pragma once

#include "Core.h"
#include "Window.h"

#include "Event/ApplicationEvent.h"

namespace Wgine {
	class WGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event &e);

	private:
		bool OnWindowClosed(WindowCloseEvent &e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// to be defined in CLIENT
	Application *CreateApplication();
}
