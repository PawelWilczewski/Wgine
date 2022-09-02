#pragma once

#include "Wgine/Core/Input.h"

namespace Wgine
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual glm::vec2 GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		//virtual glm::vec2 GetMouseDeltaRelativeImpl() override; // probs bad architecture
	};
}
