#pragma once

#include "Wgine/Layer.h"

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

	};
}

