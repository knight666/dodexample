#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Voxel::Voxel()
		: m_culled(false)
	{
	}

	void Voxel::setup(
		const glm::vec3& position, float halfSize,
		const glm::vec3& color)
	{
		m_position = position;

		m_aabbMinimum = position;
		m_aabbMinimum -= glm::vec3(halfSize);

		m_aabbMaximum = position;
		m_aabbMaximum += glm::vec3(halfSize);

		m_color = color;
	}

}; // namespace Tmpl