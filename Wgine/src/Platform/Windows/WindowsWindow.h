#pragma once

#include "Wgine/Core/Window.h"
#include "Wgine/Renderer/RenderingContext.h"

#include <GLFW/glfw3.h>

namespace Wgine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps &props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return m_Data.Width; }
		virtual unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		// TODO: better api (mouse input mode or whatever)
		virtual void SetShowMouse(const bool &show) override { glfwSetInputMode(m_Window, GLFW_CURSOR, show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED); }
		virtual bool GetShowMouse() const override { return glfwGetInputMode(m_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL; }

		inline virtual void *GetNativeWindow() const { return m_Window; };

	private:
		GLFWwindow *m_Window;
		RenderingContext *m_Context;

		struct WindowData
		{
			std::string Title = "Window";
			unsigned int Width = 1600, Height = 900;
			bool VSync = false;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
