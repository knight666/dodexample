#include "graphics/Shader.hpp"

#include "graphics/Program.hpp"

namespace Tmpl {

	Shader::Shader(Program& program, Type type)
		: m_program(program)
		, m_type(type)
	{
		m_handle = glCreateShader((GLenum)m_type);
		glAttachShader(m_program.m_handle, m_handle);
	}

	Shader::~Shader()
	{
		glDetachShader(m_program.m_handle, m_handle);
		glDeleteShader(m_handle);
	}

	bool Shader::compile(const std::string& source)
	{
		const GLchar* input = (const GLchar*)source.c_str();
		GLint input_length = (GLint)source.length();
		glShaderSource(m_handle, 1, &input, &input_length);
		glCompileShader(m_handle);

		GLint log_length = 0;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 1)
		{
			m_log.resize(log_length + 1);
			m_log.clear();
		
			glGetShaderInfoLog(m_handle, log_length, nullptr, (GLchar*)&m_log[0]);

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
		glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);

		return (success == GL_TRUE);
	}

}; // namespace Tmpl