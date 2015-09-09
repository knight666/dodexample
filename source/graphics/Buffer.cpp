#include "graphics/Buffer.hpp"

namespace Tmpl {

	Buffer::Buffer(GLenum target)
		: m_target(target)
		, m_bound(0)
	{
		glGenBuffers(1, &m_handle);
	}

	Buffer::~Buffer()
	{
		glDeleteBuffers(1, &m_handle);
	}

	void Buffer::bind()
	{
		glBindBuffer(m_target, m_handle);
		m_bound++;
	}

	void Buffer::bindBase(GLuint index)
	{
		glBindBufferBase(m_target, index, m_handle);
		m_bound++;
	}

	void Buffer::unbind()
	{
		if (m_bound > 0 &&
			--m_bound == 0)
		{
			glBindBuffer(m_target, 0);
		}
	}

	void Buffer::unmap()
	{
		glUnmapBuffer(m_target);
	}

}; // namespace Tmpl