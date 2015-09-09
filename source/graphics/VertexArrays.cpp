#include "graphics/VertexArrays.hpp"

namespace Tmpl {

	VertexArrays::VertexArrays()
	{
		glGenVertexArrays(1, &m_handle);
	}

	VertexArrays::~VertexArrays()
	{
		glDeleteVertexArrays(1, &m_handle);
	}

	void VertexArrays::bind()
	{
		glBindVertexArray(m_handle);
		m_bound++;
	}

	void VertexArrays::unbind()
	{
		if (m_bound > 0 &&
			--m_bound == 0)
		{
			glBindVertexArray(0);
		}
	}

	void VertexArrays::setAttribute(GLint attribute, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* data)
	{
		if (m_bound == 0) { return; }
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, size, type, normalized, stride, data);
	}

}; // namespace Tmpl