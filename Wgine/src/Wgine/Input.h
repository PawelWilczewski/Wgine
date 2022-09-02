#pragma once

#include "Wgine/Core.h"
#include "Wgine/InputKeyCodes.h"
#include <glm/glm.hpp>

namespace Wgine
{
	class WGINE_API Input
	{
	public:
		inline static bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }
		inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		//inline static glm::vec2 GetMouseDeltaRelative() { return s_Instance->GetMouseDeltaRelativeImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual glm::vec2 GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		//virtual glm::vec2 GetMouseDeltaRelativeImpl() = 0;

	private:
		static Input *s_Instance;
		glm::vec2 m_MousePosLastFrame = glm::vec2(0);
	};
}
