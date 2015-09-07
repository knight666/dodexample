#include "VertexArrays.hpp"

VertexArrays::VertexArrays()
{
	glGenVertexArrays(1, &_handle);
}

VertexArrays::~VertexArrays()
{
	glDeleteVertexArrays(1, &_handle);
}

void VertexArrays::bind()
{
	glBindVertexArray(_handle);
	_bound++;
}

void VertexArrays::unbind()
{
	if (_bound > 0 &&
		--_bound == 0)
	{
		glBindVertexArray(0);
	}
}

void VertexArrays::setAttribute(GLint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data)
{
	if (_bound == 0) { return; }
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, size, type, normalized, stride, data);
}