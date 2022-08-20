#include "Application.h"

#include "Event/ApplicationEvent.h"
#include "Wgine/Log.h"

namespace Wgine {
	Wgine::Application::Application()
	{
	}

	Wgine::Application::~Application()
	{
	}

	void Wgine::Application::Run()
	{
		WindowResizeEvent e(1920, 1080);
		WGINE_TRACE(e);

		while (true);
	}
}
