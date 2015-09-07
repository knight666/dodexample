#include "Program.hpp"

Program::Program()
	: _bound(0)
{
	_handle = glCreateProgram();
}

Program::~Program()
{
	glDeleteProgram(_handle);
}

void Program::bind()
{
	glUseProgram(_handle);
	_bound++;
}

void Program::unbind()
{
	if (_bound > 0 &&
		--_bound == 0)
	{
		glUseProgram(0);
	}
}

GLint Program::getUniformLocation(const char* name) const
{
	return glGetUniformLocation(_handle, (const GLchar*)name);
}

GLuint Program::getUniformBlockIndex(const char* name) const
{
	return glGetUniformBlockIndex(_handle, (const GLchar*)name);
}

void Program::setUniformBlockBinding(GLuint index, GLuint binding)
{
	glUniformBlockBinding(_handle, index, binding);
}

void Program::setUniformMatrix(GLint location, const glm::mat4x4& matrix, bool transposed /*= false*/)
{
	glUniformMatrix4fv(location, 1, (transposed) ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
}

bool Program::loadShader(Shader::Type type, const std::string& source)
{
	std::shared_ptr<Shader> shader;

	switch (type)
	{

	case Shader::Type::Vertex:
		if (_vertex == nullptr)
		{
			_vertex = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Vertex));
		}
		shader = _vertex;
		break;

	case Shader::Type::Fragment:
		if (_fragment == nullptr)
		{
			_fragment = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Fragment));
		}
		shader = _fragment;
		break;

	case Shader::Type::Geometry:
		if (_geometry == nullptr)
		{
			_geometry = std::shared_ptr<Shader>(new Shader(*this, Shader::Type::Geometry));
		}
		shader = _geometry;
		break;

	}

	return shader->compile(source);
}

bool Program::loadShaderFromFile(Shader::Type type, const std::string& path)
{
	std::fstream file_handle(path);
	if (!file_handle.is_open())
	{
		return false;
	}

	std::stringstream ss;
	ss << file_handle.rdbuf();
	file_handle.close();

	return loadShader(type, ss.str());
}

bool Program::link()
{
	if (_vertex == nullptr || _fragment == nullptr)
	{
		return false;
	}

	glLinkProgram(_handle);

	GLint log_length = 0;
	glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length > 1)
	{
		_log.resize(log_length + 1);
		_log.clear();

		glGetProgramInfoLog(_handle, log_length, nullptr, (GLchar*)&_log[0]);

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

	glGetProgramiv(_handle, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		return false;
	}

	glValidateProgram(_handle);
	glGetProgramiv(_handle, GL_VALIDATE_STATUS, &success);

	return (success == GL_TRUE);
}

GLint Program::getAttributeLocation(const char* name) const
{
	return glGetAttribLocation(_handle, (const GLchar*)name);
}

void Program::bindAttributeLocation(const char* name, AttributeLocation location)
{
	glBindAttribLocation(_handle, (GLuint)location, (const GLchar*)name);
}

GLint Program::getFragmentDataLocation(const char* name) const
{
	return glGetFragDataLocation(_handle, (const GLchar*)name);
}

void Program::bindFragmentDataLocation(const char* name, FragmentDataLocation location)
{
	glBindFragDataLocation(_handle, (GLuint)location, (const GLchar*)name);
}