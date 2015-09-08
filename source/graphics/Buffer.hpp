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

		void setData(const GLvoid* data, size_t dataSize, GLenum usage);

		template <typename DataType>
		void setData(DataType* data, size_t count, GLenum usage);

		GLvoid* mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access);

		template <typename DataType>
		DataType* mapRange(GLintptr offset, size_t count, GLbitfield access);

		void unmap();

	private:

		GLuint _handle;
		GLenum _target;
		size_t _bound;

	};

	template <typename DataType>
	inline void Buffer::setData(DataType* data, size_t count, GLenum usage)
	{
		if (_bound == 0) { return; }
		glBufferData(_target, count * sizeof(DataType), data, usage);
	}

	template <typename DataType>
	inline DataType* Buffer::mapRange(GLintptr offset, size_t count, GLbitfield access)
	{
		return (DataType*)mapRange(offset, count * sizeof(DataType), access);
	}

}; // namespace Tmpl