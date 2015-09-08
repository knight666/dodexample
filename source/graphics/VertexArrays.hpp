#pragma once

#include "base/Main.hpp"

namespace Tmpl {

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

}; // namespace Tmpl