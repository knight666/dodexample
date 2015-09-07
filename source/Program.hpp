#pragma once

#include "Base.hpp"
#include "Shader.hpp"

class Program
{

	friend class Shader;

public:

	enum class AttributeLocation
	{
		Position = 0,
		TextureCoordinate = 4,
	};

	enum class FragmentDataLocation
	{
		Color = 0,
		Red = 0,
		Green = 1,
		Blue = 2,
		Alpha = 0,
	};

	Program();
	~Program();

	const std::string& getLog() const { return _log; }

	void bind();
	void unbind();

	GLint getUniformLocation(const char* name) const;
	GLuint getUniformBlockIndex(const char* name) const;

	template <typename ValueType>
	void setUniform(GLint location, ValueType value);

	void setUniformMatrix(GLint location, const glm::mat4x4& matrix, bool transposed = false);

	void setUniformBlockBinding(GLuint index, GLuint binding);

	bool loadShader(Shader::Type type, const std::string& source);
	bool loadShaderFromFile(Shader::Type type, const std::string& path);

	bool link();

	GLint getAttributeLocation(const char* name) const;
	void bindAttributeLocation(const char* name, AttributeLocation location);

	GLint getFragmentDataLocation(const char* name) const;
	void bindFragmentDataLocation(const char* name, FragmentDataLocation location);

private:

	GLuint _handle;
	size_t _bound;
	std::shared_ptr<Shader> _vertex;
	std::shared_ptr<Shader> _fragment;
	std::shared_ptr<Shader> _geometry;
	std::string _log;

};

template <>
inline void Program::setUniform(GLint location, GLint value)
{
	if (_bound == 0) { return; }
	glUniform1i(location, value);
}

template <>
inline void Program::setUniform(GLint location, glm::mat4x4 value)
{
	if (_bound == 0) { return; }
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}