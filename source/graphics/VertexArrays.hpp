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

		GLuint m_handle;
		size_t m_bound;

	};

}; // namespace Tmpl