#pragma once

#include "Core.h"

namespace Wgine {
	class WGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// to be defined in CLIENT
	Application *CreateApplication();
}
