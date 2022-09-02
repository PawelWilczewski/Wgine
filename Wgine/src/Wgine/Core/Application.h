#pragma once

#include "Core.h"

#include "Window.h"
#include "Wgine/Core/LayerStack.h"
#include "Wgine/Event/ApplicationEvent.h"

#include "Wgine/ImGui/ImGuiLayer.h"
#include "Wgine/Renderer/Shader.h"
#include "Wgine/Renderer/Buffer.h"
#include "Wgine/Renderer/VertexArray.h"
#include "Wgine/Camera/Camera.h"

// TODO: application should be able to have multiple windows
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

		inline static Application &Get() { return *s_Instance; }
		inline Window &GetWindow() { return *m_Window; }

	private:
		bool OnWindowClosed(WindowCloseEvent &e);
		bool OnWindowResized(WindowResizeEvent &e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;

	private:
		static Application *s_Instance;
	};

	// to be defined in CLIENT
	Application *CreateApplication();
}
