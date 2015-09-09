#include "voxels/oop/LogicOOP.hpp"

namespace Tmpl {

	LogicOOP::LogicOOP()
		: m_voxelsActive(0)
		, m_voxelHalfSize(0.0f)
		, m_program(new Program())
		, m_vertices(new Buffer(GL_ARRAY_BUFFER))
		, m_attributes(new VertexArrays())
		, m_uniforms(new Buffer(GL_UNIFORM_BUFFER))
	{
	}

	LogicOOP::~LogicOOP()
	{
	}

	bool LogicOOP::initialize()
	{
		// Program

		bool validated = true;
		validated &= m_program->loadShaderFromFile(Shader::Type::Vertex, "media/shaders/voxels.vert");
		validated &= m_program->loadShaderFromFile(Shader::Type::Geometry, "media/shaders/voxels.geom");
		validated &= m_program->loadShaderFromFile(Shader::Type::Fragment, "media/shaders/voxels.frag");
		validated &= m_program->link();

		if (!validated)
		{
			return false;
		}

		// Vertices

		m_vertices->bind();

			m_vertices->setData<Vertex>(nullptr, Logic::MaxVoxelCount, GL_STREAM_DRAW);

		m_vertices->unbind();

		// Attributes

		m_attributes->bind();

			m_vertices->bind();
			m_attributes->setAttribute(m_program->getAttributeLocation("attrPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)Vertex::Offset::Position);
			m_attributes->setAttribute(m_program->getAttributeLocation("attrColor"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)Vertex::Offset::Color);

		m_attributes->unbind();

		// Uniforms

		m_uniformTransform = m_program->getUniformBlockIndex("VertexUniforms");
		m_uniformHalfSize = m_program->getUniformLocation("halfSize");

		GLint uniform_buffer_offset = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniform_buffer_offset);
		size_t uniform_buffer_size = glm::max(uniform_buffer_offset, (GLint)sizeof(Uniforms));

		m_uniforms->bind();

			m_uniforms->setData<Uniforms>(nullptr, uniform_buffer_size, GL_DYNAMIC_DRAW);

		m_uniforms->unbind();

		return true;
	}

	void LogicOOP::generateVoxels(size_t count, float halfSize)
	{
		if (count == m_voxelsActive &&
			m_voxelHalfSize == halfSize)
		{
			return;
		}

		count = std::min(count, Logic::MaxVoxelCount);
		m_voxels.resize(count);

		glm::vec3 position;
		glm::vec3 color = glm::linearRand(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f));

		int last_direction = 0;

		for (size_t i = 0; i < count; ++i)
		{
			m_voxels[i].setup(position, halfSize, color);

			int side = 0;
			int sign = 0;

			while ((side + 1) * sign == last_direction)
			{
				side = (int)glm::linearRand(0.0f, 3.0f);
				sign = (glm::linearRand(0.0f, 1.0f) > 0.5f) ? -1 : 1;
			}

			position[side] += halfSize * sign * 2.0f;

			last_direction = side * sign;

			color = glm::linearRand(
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f));
		}

		m_voxelsActive = count;
		m_voxelHalfSize = halfSize; 
	}

	void LogicOOP::update(uint32_t milliSeconds)
	{
	}

	void LogicOOP::render(const glm::mat4x4& modelViewProjection)
	{
		m_uniforms->bind();
			Uniforms* transform = m_uniforms->mapRange<Uniforms>(0, 1, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
				transform->modelViewProjection = modelViewProjection;
			m_uniforms->unmap();
		m_uniforms->unbind();

		m_vertices->bind();
			Vertex* data = m_vertices->map<Vertex>(GL_READ_WRITE);
				for (size_t i = 0; i < m_voxelsActive; ++i)
				{
					data[i].position = m_voxels[i].getPosition();
					data[i].color = m_voxels[i].getColor();
				}
			m_vertices->unmap();
		m_vertices->unbind();

		m_program->bind();

			m_uniforms->bindBase(0);
			m_program->setUniformBlockBinding(m_uniformTransform, 0);
			m_program->setUniform(m_uniformHalfSize, m_voxelHalfSize);
			
			m_attributes->bind();

			glDrawArrays(GL_POINTS, 0, m_voxelsActive);

			m_attributes->unbind();

			m_uniforms->unbind();

		m_program->unbind();
	}

}; // namespace Tmpl