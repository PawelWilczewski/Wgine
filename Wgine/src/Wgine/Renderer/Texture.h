#pragma once

#include "Wgine/Core/Core.h"
#include <string>

namespace Wgine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint8_t slot = 0) = 0;

		virtual uint32_t GetSlot() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, void *data);
		static Ref<Texture2D> Create(const std::string &path);
	};
}
