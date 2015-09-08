#include "graphics/Texture.hpp"

namespace Tmpl {

	Texture::Texture(GLenum target, GLenum format, GLsizei width, GLsizei height)
		: _target(target)
		, _format(format)
		, _width(width)
		, _height(height)
		, _bound(0)
	{
		glGenTextures(1, &_handle);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_handle);
	}

	void Texture::bind(GLuint index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(_target, _handle);
		_bound++;
	}

	void Texture::unbind()
	{
		if (_bound > 0 &&
			--_bound == 0)
		{
			glBindTexture(_target, 0);
		}
	}

	void Texture::image2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint border /*= 0*/)
	{
		if (_bound == 0) { return; }
		glTexImage2D(_target, level, _format, _width, _height, border, internalFormat, dataType, data);
	}

	void Texture::subImage2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint offsetX /*= 0*/, GLint offsetY /*= 0*/)
	{
		if (_bound == 0) { return; }
		glTexSubImage2D(_target, level, offsetX, offsetY, _width, _height, internalFormat, dataType, data);
	}

}; // namespace Tmpl