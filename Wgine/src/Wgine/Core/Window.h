#pragma once

#include "WginePCH.h"

#include "Wgine/Core/Core.h"
#include "Wgine/Event/Event.h"

namespace Wgine
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string &title = "Wgine Engine",
			unsigned int width = 1600,
			unsigned int height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class WGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event &)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetShowMouse(const bool &show) = 0;
		virtual bool GetShowMouse() const = 0;

		virtual void *GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps &props = WindowProps());
	};
}
