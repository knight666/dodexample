#pragma once

#include "base/Main.hpp"

class Texture
{

public:

	Texture(GLenum target, GLenum format, GLsizei width, GLsizei height);
	~Texture();

	GLsizei getWidth() const { return _width; }
	GLsizei getHeight() const { return _height; }

	void bind(GLuint index);
	void unbind();

	template <typename ValueType>
	void setParameter(GLenum parameter, ValueType value);

	void image2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint border = 0);

	template <typename ValueType>
	void image2D(GLint level, const ValueType* data, GLenum internalFormat, GLint border = 0);

	void subImage2D(GLint level, GLenum dataType, const GLvoid* data, GLenum internalFormat, GLint offsetX = 0, GLint offsetY = 0);

	template <typename ValueType>
	void subImage2D(GLint level, const ValueType* data, GLenum internalFormat, GLint offsetX = 0, GLint offsetY = 0);

private:

	GLuint _handle;
	GLenum _target;
	GLenum _format;
	GLsizei _width;
	GLsizei _height;
	size_t _bound;

};

template <>
inline void Texture::setParameter(GLenum parameter, GLint value)
{
	if (_bound == 0) { return; }
	glTexParameteri(_target, parameter, value);
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