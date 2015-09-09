#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	void Voxel::setup(
		const glm::vec3& position, float halfSize,
		const glm::vec4& color)
	{
		m_aabbMinimum = position;
		m_aabbMinimum -= glm::vec3(halfSize);

		m_aabbMaximum = position;
		m_aabbMaximum += glm::vec3(halfSize);
	}

}; // namespace Tmpl