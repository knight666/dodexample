#include "graphics/Texture.hpp"

namespace Tmpl {

	Texture::Texture(GLenum target, GLenum format, GLsizei width, GLsizei height)
		: m_target(target)
		, m_format(format)
		, m_width(width)
		, m_height(height)
		, m_bound(0)
	{
		glGenTextures(1, &m_handle);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_handle);
	}

	void Texture::resize(GLsizei width, GLsizei height)
	{
		if (m_width == width &&
			m_height == height)
		{
			return;
		}

		m_width = width;
		m_height = height;

		bind(0);
		image2D(0, (const GLbyte*)nullptr, GL_BGRA);
		unbind();
	}

	void Texture::bind(GLuint index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(m_target, m_handle);
		m_bound++;
	}

	void Texture::unbind()
	{
		if (m_bound > 0 &&
			--m_bound == 0)
		{
			glBindTexture(m_target, 0);
		}
	}

	std::shared_ptr<Buffer> Texture::getPixels()
	{
		if (m_bound == 0)
		{
			return nullptr;
		}

		if (m_pixels == nullptr)
		{
			m_pixels = std::shared_ptr<Buffer>(new Buffer(GL_PIXEL_UNPACK_BUFFER));
		}

		return m_pixels;
	}

	void Texture::image2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint border /*= 0*/)
	{
		if (m_bound == 0) { return; }
		glTexImage2D(m_target, level, m_format, m_width, m_height, border, internalFormat, dataType, data);
	}

	void Texture::subImage2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint offsetX /*= 0*/, GLint offsetY /*= 0*/)
	{
		if (m_bound == 0) { return; }
		glTexSubImage2D(m_target, level, offsetX, offsetY, m_width, m_height, internalFormat, dataType, data);
	}

}; // namespace Tmpl