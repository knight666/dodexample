#include "voxels/Renderer.hpp"

#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/VertexArrays.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::initialize()
	{
		// Program

		m_program = std::make_shared<Program>();

		if (!m_program->loadShaderFromFile(
			Shader::Type::Vertex,
			"media/shaders/voxels.vert"))
		{
			return false;
		}

		if (!m_program->loadShaderFromFile(
			Shader::Type::Geometry,
			"media/shaders/voxels.geom"))
		{
			return false;
		}

		if (!m_program->loadShaderFromFile(
			Shader::Type::Fragment,
			"media/shaders/voxels.frag"))
		{
			return false;
		}

		if (!m_program->link())
		{
			return false;
		}

		// Vertices

		m_vertices = std::make_shared<Buffer>(GL_ARRAY_BUFFER);
		m_vertices->bind();
			m_vertices->setData<Vertex>(
				nullptr,
				Logic::MaxVoxelCount,
				GL_STREAM_DRAW);
		m_vertices->unbind();

		// Attributes

		m_attributes = std::make_shared<VertexArrays>(m_program);
		m_attributes->bind();
			m_vertices->bind();
				m_attributes->setAttribute<glm::vec3>(
					"attrPosition",
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Position);
				m_attributes->setAttribute<glm::vec3>(
					"attrColor",
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Color);
			m_vertices->unbind();
		m_attributes->unbind();

		// Uniforms

		m_uniformTransform = m_program->getUniformBlockIndex("VertexUniforms");
		m_uniformHalfSize = m_program->getUniformLocation("halfSize");

		GLint uniform_buffer_offset = 0;
		glGetIntegerv(
			GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
			&uniform_buffer_offset);
		size_t uniform_buffer_size =
			glm::max(uniform_buffer_offset, (GLint)sizeof(Uniforms));

		m_uniforms = std::make_shared<Buffer>(GL_UNIFORM_BUFFER);
		m_uniforms->bind();
			m_uniforms->setData<Uniforms>(
				nullptr,
				uniform_buffer_size,
				GL_DYNAMIC_DRAW);
		m_uniforms->unbind();

		return true;
	}

	void Renderer::render(
		std::shared_ptr<Logic> logic,
		const Options& options,
		const glm::mat4x4& viewProjection)
	{
		m_vertices->bind();
			Vertex* data = m_vertices->map<Vertex>(GL_READ_WRITE);
				size_t used = logic->render(options, data);
			m_vertices->unmap();
		m_vertices->unbind();

		m_program->bind();

		m_program->setUniform(m_uniformHalfSize, options.voxelHalfSize);

		m_uniforms->bindBase(0);

		Uniforms* transform = m_uniforms->mapRange<Uniforms>(
			0, 1,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
		transform->modelViewProjection = viewProjection;
		m_uniforms->unmap();

		m_program->setUniformBlockBinding(m_uniformTransform, 0);

		m_attributes->bind();

		glDrawArrays(GL_POINTS, 0, used);

		m_attributes->unbind();
		m_uniforms->unbind();
		m_program->unbind();
	}

}; // namespace Tmpl