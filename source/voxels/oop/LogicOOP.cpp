#include "voxels/oop/LogicOOP.hpp"

namespace Tmpl {

	LogicOOP::LogicOOP()
		: m_voxelsActive(0)
		, m_voxelHalfSize(0.0f)
	{
	}

	LogicOOP::~LogicOOP()
	{
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
		glm::vec4 color = glm::linearRand(
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
				glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_voxelsActive = count;
		m_voxelHalfSize = halfSize; 
	}

	void LogicOOP::update(uint32_t milliSeconds)
	{
	}

	void LogicOOP::render()
	{
	}

}; // namespace Tmpl