#include "voxels/dod/LogicDOD.hpp"

namespace Tmpl {

	LogicDOD::LogicDOD()
		: m_collectionActive(0)
		, m_voxelHalfSize(0.0f)
	{
	}

	LogicDOD::~LogicDOD()
	{
	}

	bool LogicDOD::initialize()
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
				m_attributes->setAttribute(
					m_program->getAttributeLocation("attrPosition"),
					3, GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Position);
				m_attributes->setAttribute(
					m_program->getAttributeLocation("attrColor"),
					3, GL_FLOAT, GL_FALSE,
					sizeof(Vertex),
					(const GLvoid*)Vertex::Offset::Color);
			m_vertices->unbind();
		m_attributes->unbind();

		// Uniforms

		m_uniformTransform = m_program->getUniformBlockIndex("VertexUniforms");
		m_uniformHalfSize = m_program->getUniformLocation("halfSize");

		GLint uniform_buffer_offset = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniform_buffer_offset);
		size_t uniform_buffer_size = glm::max(uniform_buffer_offset, (GLint)sizeof(Uniforms));

		m_uniforms->bind();
			m_uniforms->setData<Uniforms>(
				nullptr,
				uniform_buffer_size,
				GL_DYNAMIC_DRAW);
		m_uniforms->unbind();

		return true;
	}

	void LogicDOD::setVoxels(
		const std::vector<VoxelData>& voxels,
		float halfSize)
	{
		size_t i = 0;

		for (const auto& voxel : voxels)
		{
			m_collection.voxel_position_x[i] = voxel.position.x;
			m_collection.voxel_position_y[i] = voxel.position.y;
			m_collection.voxel_position_z[i] = voxel.position.z;
			m_collection.voxel_color_r[i] = voxel.color.x;
			m_collection.voxel_color_g[i] = voxel.color.y;
			m_collection.voxel_color_b[i] = voxel.color.z;

			i++;
		}

		m_collectionActive = i;

		m_voxelHalfSize = halfSize;
	}

	size_t LogicDOD::cullVoxels(
		const Options& options,
		const glm::vec3& targetPosition)
	{
		size_t culled = 0;

		for (size_t i = 0; i < m_collectionActive; ++i)
		{
			// Normalize ray direction

			m_collection.ray_direction_x[i] = m_collection.voxel_position_x[i] - targetPosition.x;
			m_collection.ray_direction_y[i] = m_collection.voxel_position_y[i] - targetPosition.y;
			m_collection.ray_direction_z[i] = m_collection.voxel_position_z[i] - targetPosition.z;

			float length = glm::sqrt(
				(m_collection.ray_direction_x[i] * m_collection.ray_direction_x[i]) +
				(m_collection.ray_direction_y[i] * m_collection.ray_direction_y[i]) +
				(m_collection.ray_direction_z[i] * m_collection.ray_direction_z[i]));

			float lengthReciprocal = 1.0f / length;
			m_collection.ray_direction_x[i] = 1.0f / (m_collection.ray_direction_x[i] * lengthReciprocal);
			m_collection.ray_direction_y[i] = 1.0f / (m_collection.ray_direction_y[i] * lengthReciprocal);
			m_collection.ray_direction_z[i] = 1.0f / (m_collection.ray_direction_z[i] * lengthReciprocal);

			// Setup ray

			float offsetMinimum;
			float offsetMaximum;

			m_collection.ray_time_minimum[i] = 0.0f;

			offsetMinimum = ((m_collection.voxel_position_x[i] - m_voxelHalfSize) - targetPosition.x) * m_collection.ray_direction_x[i];
			offsetMaximum = ((m_collection.voxel_position_x[i] + m_voxelHalfSize) - targetPosition.x) * m_collection.ray_direction_x[i];

			m_collection.ray_time_minimum[i] = glm::max(m_collection.ray_time_minimum[i], glm::min(offsetMinimum, offsetMaximum));

			offsetMinimum = ((m_collection.voxel_position_y[i] - m_voxelHalfSize) - targetPosition.y) * m_collection.ray_direction_y[i];
			offsetMaximum = ((m_collection.voxel_position_y[i] + m_voxelHalfSize) - targetPosition.y) * m_collection.ray_direction_y[i];

			m_collection.ray_time_minimum[i] = glm::max(m_collection.ray_time_minimum[i], glm::min(offsetMinimum, offsetMaximum));

			offsetMinimum = ((m_collection.voxel_position_z[i] - m_voxelHalfSize) - targetPosition.z) * m_collection.ray_direction_z[i];
			offsetMaximum = ((m_collection.voxel_position_z[i] + m_voxelHalfSize) - targetPosition.z) * m_collection.ray_direction_z[i];

			m_collection.ray_time_minimum[i] = glm::max(m_collection.ray_time_minimum[i], glm::min(offsetMinimum, offsetMaximum));

			// Check intersections

			m_collection.voxel_culled[i] = false;

			for (size_t j = 0; j < m_collectionActive; ++j)
			{
				if (j == i)
				{
					continue;
				}

				float localTimeMinimum = 0.0f;
				float localTimeMaximum = std::numeric_limits<float>::max();

				offsetMinimum = ((m_collection.voxel_position_x[j] - m_voxelHalfSize) - targetPosition.x) * m_collection.ray_direction_x[i];
				offsetMaximum = ((m_collection.voxel_position_x[j] + m_voxelHalfSize) - targetPosition.x) * m_collection.ray_direction_x[i];

				localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum, offsetMaximum));
				localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum, offsetMaximum));

				offsetMinimum = ((m_collection.voxel_position_y[j] - m_voxelHalfSize) - targetPosition.y) * m_collection.ray_direction_y[i];
				offsetMaximum = ((m_collection.voxel_position_y[j] + m_voxelHalfSize) - targetPosition.y) * m_collection.ray_direction_y[i];

				localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum, offsetMaximum));
				localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum, offsetMaximum));

				offsetMinimum = ((m_collection.voxel_position_z[j] - m_voxelHalfSize) - targetPosition.z) * m_collection.ray_direction_z[i];
				offsetMaximum = ((m_collection.voxel_position_z[j] + m_voxelHalfSize) - targetPosition.z) * m_collection.ray_direction_z[i];

				localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum, offsetMaximum));
				localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum, offsetMaximum));

				if (localTimeMinimum <= localTimeMaximum &&
					localTimeMinimum <= m_collection.ray_time_minimum[i])
				{
					m_collection.voxel_culled[i] = true;
					culled++;

					break;
				}
			}
		}

		return culled;
	}

	void LogicDOD::render(
		const Options& options,
		const glm::mat4x4& modelViewProjection)
	{
		m_vertices->bind();
		Vertex* data = m_vertices->map<Vertex>(GL_READ_WRITE);
		Vertex* data_dst = data;
		size_t data_count = 0;

		if (!options.culling)
		{
			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				data[i].position.x = m_collection.voxel_position_x[i];
				data[i].position.y = m_collection.voxel_position_y[i];
				data[i].position.z = m_collection.voxel_position_z[i];
				data[i].color.r = m_collection.voxel_color_r[i];
				data[i].color.g = m_collection.voxel_color_g[i];
				data[i].color.b = m_collection.voxel_color_b[i];
			}

			data_count = m_collectionActive;
		}
		else if (
			options.showCulled)
		{
			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				data[i].position.x = m_collection.voxel_position_x[i];
				data[i].position.y = m_collection.voxel_position_y[i];
				data[i].position.z = m_collection.voxel_position_z[i];

				if (m_collection.voxel_culled[i])
				{
					data[i].color.r = 1.0f;
					data[i].color.g = 0.0f;
					data[i].color.b = 0.0f;
				}
				else
				{
					data[i].color.r = 0.0f;
					data[i].color.g = 1.0f;
					data[i].color.b = 0.0f;
				}
			}

			data_count = m_collectionActive;
		}
		else
		{
			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				if (!m_collection.voxel_culled[i])
				{
					data_dst->position.x = m_collection.voxel_position_x[i];
					data_dst->position.y = m_collection.voxel_position_y[i];
					data_dst->position.z = m_collection.voxel_position_z[i];
					data_dst->color.x = m_collection.voxel_color_r[i];
					data_dst->color.y = m_collection.voxel_color_g[i];
					data_dst->color.z = m_collection.voxel_color_b[i];

					data_dst++;
					data_count++;
				}
			}
		}

		m_vertices->unmap();
		m_vertices->unbind();

		m_program->bind();

		m_program->setUniform(m_uniformHalfSize, m_voxelHalfSize);

		m_uniforms->bindBase(0);

		Uniforms* transform = m_uniforms->mapRange<Uniforms>(
			0, 1,
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			transform->modelViewProjection = modelViewProjection;
		m_uniforms->unmap();

		m_program->setUniformBlockBinding(m_uniformTransform, 0);

		m_attributes->bind();

		glDrawArrays(
			GL_POINTS,
			0,
			data_count);

		m_attributes->unbind();
		m_uniforms->unbind();
		m_program->unbind();
	}

}; // namespace Tmpl