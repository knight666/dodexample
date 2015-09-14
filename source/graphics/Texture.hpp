#pragma once

#include "base/Main.hpp"
#include "graphics/Buffer.hpp"

namespace Tmpl {

	class Texture
	{

	public:

		Texture(GLenum target, GLenum format, GLsizei width, GLsizei height);
		~Texture();

		GLsizei getWidth() const { return m_width; }
		GLsizei getHeight() const { return m_height; }

		void resize(GLsizei width, GLsizei height);

		void bind(GLuint index);
		void unbind();

		std::shared_ptr<Buffer> getPixels();

		template <typename ValueType>
		void setParameter(GLenum parameter, ValueType value);

		void image2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint border = 0);

		template <typename ValueType>
		void image2D(GLint level, const ValueType* data, GLenum internalFormat, GLint border = 0);

		void subImage2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint offsetX = 0, GLint offsetY = 0);

		template <typename ValueType>
		void subImage2D(GLint level, const ValueType* data, GLenum internalFormat, GLint offsetX = 0, GLint offsetY = 0);

	private:

		GLuint m_handle;
		GLenum m_target;
		GLenum m_format;
		GLsizei m_width;
		GLsizei m_height;
		size_t m_bound;
		std::shared_ptr<Buffer> m_pixels;

	};

	template <>
	inline void Texture::setParameter(GLenum parameter, GLint value)
	{
		if (m_bound == 0) { return; }
		glTexParameteri(m_target, parameter, value);
	}

	template <>
	inline void Texture::image2D(GLint level, const GLbyte* data, GLenum internalFormat, GLint border /*= 0*/)
	{
		image2D(level, GL_UNSIGNED_BYTE, data, internalFormat, border);
	}

	template <>
	inline void Texture::image2D(GLint level, const GLuint* data, GLenum internalFormat, GLint border /*= 0*/)
	{
		image2D(level, GL_UNSIGNED_INT, data, internalFormat, border);
	}

	template <>
	inline void Texture::subImage2D(GLint level, const GLbyte* data, GLenum internalFormat, GLint offsetX /*= 0*/, GLint offsetY /*= 0*/)
	{
		subImage2D(level, GL_UNSIGNED_BYTE, data, internalFormat, offsetX, offsetY);
	}

	template <>
	inline void Texture::subImage2D(GLint level, const GLuint* data, GLenum internalFormat, GLint offsetX /*= 0*/, GLint offsetY /*= 0*/)
	{
		subImage2D(level, GL_UNSIGNED_INT, data, internalFormat, offsetX, offsetY);
	}

}; // namespace Tmpl