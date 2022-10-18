#include "WginePCH.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Wgine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, void *data)
		: m_Width(width), m_Height(height)
	{
		GLenum internalFormat = GL_RGBA8, dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string &path)
		: m_Path(path)
	{
		int width, height, channels;
		stbi_uc *image = stbi_load(path.c_str(), &width, &height, &channels, 0);

		WGINE_CORE_ASSERT(image, "Image loading failure!");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat, dataFormat;
		switch (channels)
		{
		case 1:
			internalFormat = GL_R8;
			dataFormat = GL_RED;
			break;
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		default:
			WGINE_CORE_ASSERT(false, "Unsupported image type with {0} channels!", channels);
			internalFormat = GL_R8; // hypothetically we're not gonna run out of data if use just one channel
			dataFormat = GL_RED;
		}
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, image);

		stbi_image_free(image);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint8_t slot)
	{
		glBindTextureUnit(slot, m_RendererID);
		m_Slot = slot;
	}
}
