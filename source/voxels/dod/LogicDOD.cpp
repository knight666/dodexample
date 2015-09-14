#include "voxels/dod/LogicDOD.hpp"

namespace Tmpl {

	LogicDOD::LogicDOD(float halfSize)
		: Logic(halfSize)
		, m_collectionActive(0)
	{
	}

	LogicDOD::~LogicDOD()
	{
	}

	bool LogicDOD::initialize(
		const std::vector<VoxelData>& voxels)
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

		return true;
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

	size_t LogicDOD::render(
		const Options& options,
		Vertex* target)
	{
		Vertex* dst = target;

		if (!options.culling)
		{
			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				dst->position.x = m_collection.voxel_position_x[i];
				dst->position.y = m_collection.voxel_position_y[i];
				dst->position.z = m_collection.voxel_position_z[i];
				dst->color.r = m_collection.voxel_color_r[i];
				dst->color.g = m_collection.voxel_color_g[i];
				dst->color.b = m_collection.voxel_color_b[i];

				dst++;
			}

			return m_collectionActive;
		}
		else if (
			options.showCulled)
		{
			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				dst->position.x = m_collection.voxel_position_x[i];
				dst->position.y = m_collection.voxel_position_y[i];
				dst->position.z = m_collection.voxel_position_z[i];

				if (m_collection.voxel_culled[i])
				{
					dst->color.r = 1.0f;
					dst->color.g = 0.0f;
					dst->color.b = 0.0f;
				}
				else
				{
					dst->color.r = 0.0f;
					dst->color.g = 1.0f;
					dst->color.b = 0.0f;
				}

				dst++;
			}

			return m_collectionActive;
		}
		else
		{
			size_t used = 0;

			for (size_t i = 0; i < m_collectionActive; ++i)
			{
				if (!m_collection.voxel_culled[i])
				{
					dst->position.x = m_collection.voxel_position_x[i];
					dst->position.y = m_collection.voxel_position_y[i];
					dst->position.z = m_collection.voxel_position_z[i];
					dst->color.x = m_collection.voxel_color_r[i];
					dst->color.y = m_collection.voxel_color_g[i];
					dst->color.z = m_collection.voxel_color_b[i];

					dst++;
					used++;
				}
			}

			return used;
		}
	}

}; // namespace Tmpl