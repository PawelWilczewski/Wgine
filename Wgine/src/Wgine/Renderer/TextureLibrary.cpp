#include "WginePCH.h"
#include "TextureLibrary.h"

namespace Wgine
{
	static std::unordered_map<uint32_t, Ref<Texture2D>> s_ConstLibrary;
	static std::unordered_map<std::string, Ref<Texture2D>> s_FileLibrary;

	Ref<Texture2D> TextureLibrary::Get(const std::string &filePath)
	{
		if (s_FileLibrary.find(filePath) != s_FileLibrary.end())
			return s_FileLibrary[filePath];

		return s_FileLibrary[filePath] = Texture2D::Create(filePath);
	}

	Ref<Texture2D> TextureLibrary::Get(uint32_t color)
	{
		if (s_ConstLibrary.find(color) != s_ConstLibrary.end())
			return s_ConstLibrary[color];

		return s_ConstLibrary[color] = Texture2D::Create(1, 1, &color);
	}
}
