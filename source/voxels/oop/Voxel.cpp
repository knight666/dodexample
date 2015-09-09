#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Voxel::Voxel(const glm::vec3& position, float halfSize)
	{
		m_aabbMinimum = position;
		m_aabbMinimum -= glm::vec3(halfSize);

		m_aabbMaximum = position;
		m_aabbMaximum += glm::vec3(halfSize);
	}

	Voxel::~Voxel()
	{
	}

}; // namespace Tmpl