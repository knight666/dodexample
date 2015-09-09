#include "voxels/oop/Ray.hpp"

#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Ray::Ray(Voxel& owner, const glm::vec3& origin, const glm::vec3& direction)
		: m_owner(owner)
		, m_origin(origin)
		, m_direction(direction)
		, m_timeMinimum(std::numeric_limits<float>::max())
		, m_timeMaximum(0.0f)
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

		float localTimeMinimum = 0.0f;
		localTimeMinimum = glm::max(localTimeMinimum, distanceMinimum.x);
		localTimeMinimum = glm::max(localTimeMinimum, distanceMinimum.y);
		localTimeMinimum = glm::max(localTimeMinimum, distanceMinimum.z);

		float localTimeMaximum = std::numeric_limits<float>::max();
		localTimeMaximum = glm::min(localTimeMaximum, distanceMaximum.x);
		localTimeMaximum = glm::min(localTimeMaximum, distanceMaximum.y);
		localTimeMaximum = glm::min(localTimeMaximum, distanceMaximum.z);

		if (localTimeMinimum <= localTimeMaximum &&
			localTimeMinimum <= m_timeMinimum)
		{
			m_timeMinimum = localTimeMinimum;

			return true;
		}

		return false;
	}

}; // namespace Tmpl