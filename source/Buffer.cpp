#include "Buffer.hpp"

Buffer::Buffer(GLenum target) : _target(target)
	, _bound(0)
{
	glGenBuffers(1, &_handle);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &_handle);
}

void Buffer::bind()
{
	glBindBuffer(_target, _handle);
	_bound++;
}

void Buffer::bindBase(GLuint index)
{
	glBindBufferBase(_target, index, _handle);
	_bound++;
}

void Buffer::unbind()
{
	if (_bound > 0 &&
		--_bound == 0)
	{
		glBindBuffer(_target, 0);
	}
}

void Buffer::setData(const GLvoid* data, size_t dataSize, GLenum usage)
{
	if (_bound == 0) { return; }
	glBufferData(_target, dataSize, data, usage);
}

GLvoid* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
	if (_bound == 0) { return nullptr; }
	return glMapBufferRange(_target, offset, length, access);
}

void Buffer::unmap()
{
	glUnmapBuffer(_target);
}