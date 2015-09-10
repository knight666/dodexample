#pragma once

#include "base/Main.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/Texture.hpp"
#include "graphics/VertexArrays.hpp"

namespace Tmpl {

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

		GLsizei getTextureWidth() const { return m_texture->getWidth(); }
		GLsizei getTextureHeight() const { return m_texture->getHeight(); }

		void setTextureData(const GLuint* pixels, GLsizei width, GLsizei height);

		void render(const glm::mat4x4& modelViewProjection);

	private:

		std::shared_ptr<Texture> m_texture;
		std::shared_ptr<Buffer> m_vertices;
		std::shared_ptr<Buffer> m_elements;
		std::shared_ptr<Program> m_program;
		std::shared_ptr<VertexArrays> m_attributes;
		std::shared_ptr<Buffer> m_uniforms;
		GLint m_uniformTransform;
		GLint m_uniformDiffuse;

	};

}; // namespace Tmpl