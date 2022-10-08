#pragma once

#include "Texture.h"

#include <unordered_map>

namespace Wgine
{
	class TextureLibrary
	{
	public:
		static Ref<Texture2D> Get(const std::string &filePath);
		static Ref<Texture2D> Get(uint32_t color);

		static Ref<Texture2D> GetWhite() { return Get(0xffffffff); }
		static Ref<Texture2D> GetBlack() { return Get(0x000000ff); }
		static Ref<Texture2D> GetTransparent() { return Get(0x00000000); }
	};
}
