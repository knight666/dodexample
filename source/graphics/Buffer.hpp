#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Buffer
	{

	public:

		Buffer(GLenum target);

		~Buffer();

		void bind();
		void bindBase(GLuint index);
		void unbind();

		template <typename DataType>
		void setData(DataType* data, size_t count, GLenum usage);

		template <typename DataType>
		DataType* map(GLbitfield access);

		template <typename DataType>
		DataType* mapRange(GLintptr offset, size_t count, GLbitfield access);

		void unmap();

	private:

		GLuint m_handle;
		GLenum m_target;
		size_t m_bound;

	};

	template <typename DataType>
	inline void Buffer::setData(DataType* data, size_t count, GLenum usage)
	{
		if (m_bound == 0) { return; }
		glBufferData(m_target, count * sizeof(DataType), data, usage);
	}

	template <typename DataType>
	inline DataType* Buffer::map(GLbitfield access)
	{
		if (m_bound == 0) { return nullptr; }
		return (DataType*)glMapBuffer(m_target, access);
	}

	template <typename DataType>
	inline DataType* Buffer::mapRange(GLintptr offset, size_t count, GLbitfield access)
	{
		if (m_bound == 0) { return nullptr; }
		return (DataType*)glMapBufferRange(m_target, offset, count * sizeof(DataType), access);
	}

}; // namespace Tmpl