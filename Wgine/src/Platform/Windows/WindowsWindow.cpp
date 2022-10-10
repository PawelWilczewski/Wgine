#include "WginePCH.h"
#include "WindowsWindow.h"

#include "Wgine/Event/ApplicationEvent.h"
#include "Wgine/Event/MouseEvent.h"
#include "Wgine/Event/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

#include <glm/glm.hpp>

namespace Wgine
{
	static bool s_GLFWInitialized = false;

	static void glfwErrorCallback(int code, const char *desc)
	{
		WGINE_CORE_ERROR("GLFW ERRROR: CODE {0}, {1}", code, desc);
	}

	std::unique_ptr<Window> Window::Create(const WindowProps &props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps &props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		WGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		// TODO: window should not be initializing GLFW directly
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			WGINE_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(glfwErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = new OpenGLContext(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		// set callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
			{
				WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent e(width, height);
				data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			WindowCloseEvent e;
			data.EventCallback(e);
			});

		// TODO: convert key code to some engine standard
		glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1); // TODO: correct repeat count
					data.EventCallback(e);
					break;
				}
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keyCode) {
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(keyCode);
			data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
			{
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent e(button);
						data.EventCallback(e);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent e(button);
						data.EventCallback(e);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
			{
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				data.EventCallback(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double x, double y) {
			WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

			MouseMovedEvent e(glm::vec2((float)x, (float)y));
			data.EventCallback(e);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}
