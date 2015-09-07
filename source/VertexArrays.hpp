#pragma once

#include "Base.hpp"

class VertexArrays
{

public:

	VertexArrays();
	~VertexArrays();

	void bind();
	void unbind();

	void setAttribute(GLint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data);

private:

	GLuint _handle;
	size_t _bound;

};