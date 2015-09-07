#include "Shader.hpp"

#include "Program.hpp"

Shader::Shader(Program& program, Type type)
	: _program(program)
	, _type(type)
{
	_handle = glCreateShader((GLenum)_type);
	glAttachShader(_program._handle, _handle);
}

Shader::~Shader()
{
	glDetachShader(_program._handle, _handle);
	glDeleteShader(_handle);
}

bool Shader::compile(const std::string& source)
{
	const GLchar* input = (const GLchar*)source.c_str();
	GLint input_length = (GLint)source.length();
	glShaderSource(_handle, 1, &input, &input_length);
	glCompileShader(_handle);

	GLint log_length = 0;
	glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		_log.resize(log_length + 1);
		_log.clear();
		
		glGetShaderInfoLog(_handle, log_length, nullptr, (GLchar*)&_log[0]);

		// some video card manufacturers (Intel) like to use the log to
		// output a message explicitly saying that there were... no errors.
		// we skip this message because it's specific to this manufacturer
		// and it's never ever going to be useful information

		if (_log == "No errors.\n")
		{
			_log.clear();
		}
	}

	GLint success = GL_TRUE;
	glGetShaderiv(_handle, GL_COMPILE_STATUS, &success);

	return (success == GL_TRUE);
}