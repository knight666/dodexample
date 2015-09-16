#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Program;

	template <typename DataType>
	class VertexAttribute
	{

	public:

		static GLenum type;
		static GLint count;

	};

	GLenum VertexAttribute<int>::type = GL_INT;
	GLint VertexAttribute<int>::count = 1;

	GLenum VertexAttribute<float>::type = GL_FLOAT;
	GLint VertexAttribute<float>::count = 1;

	GLenum VertexAttribute<glm::vec2>::type = GL_FLOAT;
	GLint VertexAttribute<glm::vec2>::count = 2;

	GLenum VertexAttribute<glm::vec3>::type = GL_FLOAT;
	GLint VertexAttribute<glm::vec3>::count = 3;

	GLenum VertexAttribute<glm::vec4>::type = GL_FLOAT;
	GLint VertexAttribute<glm::vec4>::count = 4;

	class VertexArrays
	{

	public:

		VertexArrays(std::shared_ptr<Program> program);
		~VertexArrays();

		void bind();
		void unbind();

		template <typename DataType>
		void setAttribute(
			const char* name,
			GLboolean normalized,
			GLsizei stride,
			const GLvoid* offset);

		void setAttribute(
			GLint attribute,
			GLint size,
			GLenum type,
			GLboolean normalized,
			GLsizei stride,
			const GLvoid* offset);

	private:

		std::shared_ptr<Program> m_program;
		GLuint m_handle;
		size_t m_bound;

	};

	template <typename DataType>
	void VertexArrays::setAttribute(
		const char* name,
		GLboolean normalized,
		GLsizei stride,
		const GLvoid* offset)
	{
		setAttribute(
			m_program->getAttributeLocation(name),
			VertexAttribute<DataType>::count,
			VertexAttribute<DataType>::type,
			normalized,
			stride,
			offset);
	}

}; // namespace Tmpl