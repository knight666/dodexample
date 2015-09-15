#pragma once

#include "base/Main.hpp"
#include "voxels/Options.hpp"

namespace Tmpl {

	class Buffer;
	class Logic;
	class Program;
	class VertexArrays;

	class Renderer
	{

	public:

		struct Vertex
		{
			enum class Offset
			{
				Position = 0,
				Color = sizeof(glm::vec3)
			};

			glm::vec3 position;
			glm::vec3 color;
		};

		struct Uniforms
		{
			glm::mat4x4 modelViewProjection;
		};

	public:

		Renderer();
		~Renderer();

		bool initialize();

		void render(
			std::shared_ptr<Logic> logic,
			const Options& options,
			const glm::mat4x4& viewProjection);

	private:

		std::shared_ptr<Program> m_program;
		std::shared_ptr<Buffer> m_vertices;
		std::shared_ptr<VertexArrays> m_attributes;
		std::shared_ptr<Buffer> m_uniforms;
		GLint m_uniformTransform;
		GLint m_uniformHalfSize;

	};

}; // namespace Tmpl