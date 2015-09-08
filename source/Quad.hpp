#pragma once

#include "base/Main.hpp"
#include "Buffer.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "VertexArrays.hpp"

class Quad
{

	struct Vertex
	{
		enum class Offset
		{
			Position = 0,
			TextureCoordinate = sizeof(glm::vec2)
		};

		glm::vec2 position;
		glm::vec2 texture_coordinate;
	};

	struct Uniforms
	{
		glm::mat4x4 modelViewProjection;
	};

public:

	Quad();
	~Quad();

	GLsizei getTextureWidth() const { return _texture->getWidth(); }
	GLsizei getTextureHeight() const { return _texture->getHeight(); }

	void setTextureData(const GLuint* pixels, GLsizei width, GLsizei height);

	void render(const glm::mat4x4& camera);

private:

	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Buffer> _vertices;
	std::shared_ptr<Buffer> _elements;
	std::shared_ptr<Program> _program;
	GLint _uniformTransform;
	GLint _uniformDiffuse;
	std::shared_ptr<VertexArrays> _attributes;
	std::shared_ptr<Buffer> _uniforms;

};