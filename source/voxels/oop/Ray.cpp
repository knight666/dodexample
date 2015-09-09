#include "voxels/oop/Ray.hpp"

#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Ray::Ray(Voxel& owner, const glm::vec3& origin, const glm::vec3& direction)
		: m_owner(owner)
		, m_origin(origin)
		, m_direction(direction)
		, m_timeMinimum(0.0f)
		, m_timeMaximum(std::numeric_limits<float>::max())
	{
	}

	Ray::~Ray()
	{
	}

	bool Ray::intersects(Voxel& other)
	{
		if (&other == &m_owner)
		{
			return false;
		}

		glm::vec3 localMinimum = (other.getAABBMinimum() - m_origin) * m_direction;
		glm::vec3 localMaximum = (other.getAABBMaximum() - m_origin) * m_direction;

		glm::vec3 distanceMinimum = glm::min(localMinimum, localMaximum);
		glm::vec3 distanceMaximum = glm::max(localMinimum, localMaximum);

		m_timeMinimum = glm::max(m_timeMinimum, distanceMinimum.x);
		m_timeMinimum = glm::max(m_timeMinimum, distanceMinimum.y);
		m_timeMinimum = glm::max(m_timeMinimum, distanceMinimum.z);

		m_timeMaximum = glm::min(m_timeMaximum, distanceMaximum.x);
		m_timeMaximum = glm::min(m_timeMaximum, distanceMaximum.y);
		m_timeMaximum = glm::min(m_timeMaximum, distanceMaximum.z);

		return (m_timeMinimum < m_timeMaximum);
	}

}; // namespace Tmpl