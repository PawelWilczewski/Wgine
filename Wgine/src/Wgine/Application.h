#pragma once

#include "Core.h"

#include "Window.h"
#include "Wgine/LayerStack.h"
#include "Event/ApplicationEvent.h"

#include "Wgine/ImGui/ImGuiLayer.h"
#include "Wgine/Renderer/Shader.h"
#include "Wgine/Renderer/Buffer.h"
#include "Wgine/Renderer/VertexArray.h"
#include "Wgine/Camera.h"

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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer *m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;

		Camera m_Camera;

		glm::vec3 m_CameraPosition;

	private:
		static Application *s_Instance;
	};

	// to be defined in CLIENT
	Application *CreateApplication();
}
