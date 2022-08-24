#include "WginePCH.h"
#include "ImGuiLayer.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Wgine/Application.h"
#include "Wgine/Event/KeyEvent.h"
#include "Wgine/Event/MouseEvent.h"

#include "GLFW/glfw3.h"
#include "imgui.h"

namespace Wgine
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO &io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TODO: temporary, use own key codes instead
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO &io = ImGui::GetIO();
		Application &app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0 ? time - m_Time : (1.0f / 60.f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		WGINE_CORE_TRACE("Rendering ImGui, delta time: {0}s", io.DeltaTime);
	}

	void ImGuiLayer::OnEvent(Event &event)
	{
		ImGuiIO &io = ImGui::GetIO();
		/*auto ed = EventDispatcher(event);
		ed.Dispatch(WGINE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));*/

		switch (event.GetEventType())
		{
		case Wgine::EventType::None:
			break;
		case Wgine::EventType::WindowClose:
			break;
		case Wgine::EventType::WindowResize:
			break;
		case Wgine::EventType::WindowFocus:
			break;
		case Wgine::EventType::WindowLostFocus:
			break;
		case Wgine::EventType::WindowMoved:
			break;
		case Wgine::EventType::AppTick:
			break;
		case Wgine::EventType::AppUpdate:
			break;
		case Wgine::EventType::AppRender:
			break;
		case Wgine::EventType::KeyPressed:
		{
			auto &e = dynamic_cast<KeyPressedEvent &>(event);
			io.KeysDown[e.GetKeyCode()] = true;
			break;
		}
		case Wgine::EventType::KeyReleased:
		{
			auto &e = dynamic_cast<KeyReleasedEvent &>(event);
			io.KeysDown[e.GetKeyCode()] = false;
			break;
		}
		case Wgine::EventType::MouseButtonPressed:
		{
			auto &e = dynamic_cast<MouseButtonPressedEvent &>(event);
			io.MouseDown[e.GetMouseButton()] = true;
			break;
		}
		case Wgine::EventType::MouseButtonReleased:
		{
			auto &e = dynamic_cast<MouseButtonReleasedEvent &>(event);
			io.MouseDown[e.GetMouseButton()] = false;
			break;
		}
		case Wgine::EventType::MouseMoved:
		{
			auto &e = dynamic_cast<MouseMovedEvent &>(event);
			io.MousePos = ImVec2(e.GetX(), e.GetY());
			break;
		}
		case Wgine::EventType::MouseScrolled:
		{
			auto &e = dynamic_cast<MouseScrolledEvent &>(event);
			io.MouseWheel = e.GetOffsetY();
			io.MouseWheelH = e.GetOffsetX();
			break;
		}
		default:
			break;
		}

		//event.m_Handled = true;
	}

	//bool ImGuiLayer::OnKeyPressed(KeyPressedEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnMouseButtonReleased(MouseButtonReleasedEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnMouseMoved(MouseMovedEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent &e)
	//{
	//	return false;
	//}

	//bool ImGuiLayer::OnWindowResized(WindowResizeEvent &e)
	//{
	//	return false;
	//}
}
