#include "WginePCH.h"
#include "WindowsInput.h"

#include "Wgine/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Wgine
{
	Input *Input::s_Instance = new WindowsInput();

	// ------------------------------------------------------------------------------------
	// NO NEED TO CONVERT INPUT KEY CODES BECAUASE BY DEFAUTL THEY ARE COMPATIBLE WITH GLFW
	// ------------------------------------------------------------------------------------

	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	glm::vec2 WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float WindowsInput::GetMouseXImpl()
	{
		return GetMousePositionImpl().x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		return GetMousePositionImpl().y;
	}

	//glm::vec2 WindowsInput::GetMouseDeltaRelativeImpl()
	//{

	//}
}
