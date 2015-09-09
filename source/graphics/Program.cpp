#include "graphics/Program.hpp"

namespace Tmpl {

	Program::Program()
		: m_bound(0)
	{
		m_handle = glCreateProgram();

		TMPL_LOG_INFO(Program) << m_handle << ": Created.";
	}

	Program::~Program()
	{
		TMPL_LOG_INFO(Program) << m_handle << ": Deleting.";

		glDeleteProgram(m_handle);
	}

	void Program::bind()
	{
		glUseProgram(m_handle);
		m_bound++;
	}

	void Program::unbind()
	{
		if (m_bound > 0 &&
			--m_bound == 0)
		{
			glUseProgram(0);
		}
	}

	GLint Program::getUniformLocation(const char* name) const
	{
		return glGetUniformLocation(m_handle, (const GLchar*)name);
	}

	GLuint Program::getUniformBlockIndex(const char* name) const
	{
		return glGetUniformBlockIndex(m_handle, (const GLchar*)name);
	}

	void Program::setUniformBlockBinding(GLuint index, GLuint binding)
	{
		glUniformBlockBinding(m_handle, index, binding);
	}

	void Program::setUniformMatrix(GLint location, const glm::mat4x4& matrix, bool transposed /*= false*/)
	{
		glUniformMatrix4fv(location, 1, (transposed) ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
	}

	bool Program::loadShader(Shader::Type type, const std::string& source)
	{
		TMPL_LOG_INFO(Program) << m_handle << ": Loading " << Shader::TypeToString(type)
			<< " shader from source.";

		std::shared_ptr<Shader> shader;

		switch (type)
		{

		case Shader::Type::Vertex:
			if (m_vertex == nullptr)
			{
				m_vertex = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Vertex));
			}
			shader = m_vertex;
			break;

		case Shader::Type::Fragment:
			if (m_fragment == nullptr)
			{
				m_fragment = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Fragment));
			}
			shader = m_fragment;
			break;

		case Shader::Type::Geometry:
			if (m_geometry == nullptr)
			{
				m_geometry = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Geometry));
			}
			shader = m_geometry;
			break;

		}

		return shader->compile(source);
	}

	bool Program::loadShaderFromFile(Shader::Type type, const std::string& path)
	{
		TMPL_LOG_INFO(Program) << m_handle << ": Loading " << Shader::TypeToString(type)
			<< " shader from \"" << path << "\".";

		std::fstream file_handle(path);
		if (!file_handle.is_open())
		{
			TMPL_LOG_ERROR(Program) << m_handle << ": Failed to open file.";

			return false;
		}

		std::stringstream ss;
		ss << file_handle.rdbuf();
		file_handle.close();

		return loadShader(type, ss.str());
	}

	bool Program::link()
	{
		if (m_vertex == nullptr || m_fragment == nullptr)
		{
			return false;
		}

		TMPL_LOG_INFO(Program) << m_handle << ": Linking.";

		glLinkProgram(m_handle);

		GLint log_length = 0;
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 1)
		{
			m_log.resize(log_length + 1);
			m_log.clear();

			glGetProgramInfoLog(m_handle, log_length, nullptr, (GLchar*)&m_log[0]);

			// some video card manufacturers (Intel) like to use the log to
			// output a message explicitly saying that there were... no errors.
			// we skip this message because it's specific to this manufacturer
			// and it's never ever going to be useful information

			if (m_log == "No errors.\n")
			{
				m_log.clear();
			}
		}

		GLint success = GL_TRUE;

		glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			TMPL_LOG_ERROR(Program) << m_handle << ": Failed to link.";

			return false;
		}

		glValidateProgram(m_handle);
		glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &success);

		if (success != GL_TRUE)
		{
			TMPL_LOG_ERROR(Program) << m_handle << ": Program was not validated.";

			return false;
		}

		return true;
	}

	GLint Program::getAttributeLocation(const char* name) const
	{
		return glGetAttribLocation(m_handle, (const GLchar*)name);
	}

	void Program::bindAttributeLocation(const char* name, AttributeLocation location)
	{
		glBindAttribLocation(m_handle, (GLuint)location, (const GLchar*)name);
	}

	GLint Program::getFragmentDataLocation(const char* name) const
	{
		return glGetFragDataLocation(m_handle, (const GLchar*)name);
	}

	void Program::bindFragmentDataLocation(const char* name, FragmentDataLocation location)
	{
		glBindFragDataLocation(m_handle, (GLuint)location, (const GLchar*)name);
	}

}; // namespace Tmpl