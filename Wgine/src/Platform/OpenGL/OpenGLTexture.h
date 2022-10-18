#pragma once

#include "Wgine/Renderer/Texture.h"

#include "glad/glad.h"

namespace Wgine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, void *data); // TODO: also specify the format etc.
		OpenGLTexture2D(const std::string &path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint8_t slot = 1) override;

		virtual uint32_t GetSlot() const override { return m_Slot; }

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		GLuint m_RendererID;
		uint8_t m_Slot = 0;
	};
}
