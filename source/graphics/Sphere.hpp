#pragma once

#include "base/Main.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/VertexArrays.hpp"

namespace Tmpl {

	class Sphere
	{

		struct Vertex
		{
			enum class Offset
			{
				Position = 0
			};

			Vertex(glm::vec3& position)
				: position(position)
			{
			}

			glm::vec3 position;
		};

		struct Uniforms
		{
			glm::mat4x4 modelViewProjection;
		};

	public:

		Sphere();

		void setup(size_t subdivisionsZ, size_t subdivisionsY);

		void render(
			const glm::mat4x4& viewProjection,
			const glm::vec3& position,
			float scale = 1.0f,
			const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));

	private:

		std::vector<Vertex> m_vertexData;
		std::shared_ptr<Buffer> m_vertices;
		std::vector<GLuint> m_elementData;
		std::shared_ptr<Buffer> m_elements;
		std::shared_ptr<Program> m_program;
		std::shared_ptr<VertexArrays> m_attributes;
		std::shared_ptr<Buffer> m_uniforms;
		GLint m_uniformTransform;
		GLint m_uniformColor;

	};

}; // namespace Tmpl