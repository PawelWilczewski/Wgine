#include "WginePCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Wgine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_uc *image = stbi_load(path.c_str(), &width, &height, &channels, 0);

		WGINE_CORE_ASSERT(image, "Image loading failure!");

		m_Width = width;
		m_Height = height;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, GL_RGBA8, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);

		stbi_image_free(image);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
