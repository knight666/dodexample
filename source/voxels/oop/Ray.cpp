#include "voxels/oop/Ray.hpp"

#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Ray::Ray()
		: m_owner(nullptr)
		, m_timeMinimum(0.0f)
	{
	}

	void Ray::setup(
		Voxel& owner,
		const glm::vec3& origin,
		const glm::vec3& direction)
	{
		m_owner = &owner;
		m_origin = origin;
		m_direction = direction;

		glm::vec3 offsetMinimum = (owner.getAABBMinimum() - origin) * direction;
		glm::vec3 offsetMaximum = (owner.getAABBMaximum() - origin) * direction;

		m_timeMinimum = 0.f;
		m_timeMinimum = glm::max(m_timeMinimum, glm::min(offsetMinimum.x, offsetMaximum.x));
		m_timeMinimum = glm::max(m_timeMinimum, glm::min(offsetMinimum.y, offsetMaximum.y));
		m_timeMinimum = glm::max(m_timeMinimum, glm::min(offsetMinimum.z, offsetMaximum.z));
	}

	Voxel* Ray::intersects(Voxel& other)
	{
		float localTimeMinimum = 0.f;
		float localTimeMaximum = std::numeric_limits<float>::max();

		glm::vec3 offsetMinimum = (other.getAABBMinimum() - m_origin) * m_direction;
		glm::vec3 offsetMaximum = (other.getAABBMaximum() - m_origin) * m_direction;

		localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum.x, offsetMaximum.x));
		localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum.x, offsetMaximum.x));

		localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum.y, offsetMaximum.y));
		localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum.y, offsetMaximum.y));

		localTimeMinimum = glm::max(localTimeMinimum, glm::min(offsetMinimum.z, offsetMaximum.z));
		localTimeMaximum = glm::min(localTimeMaximum, glm::max(offsetMinimum.z, offsetMaximum.z));

		if (localTimeMinimum <= localTimeMaximum &&
			localTimeMinimum <= m_timeMinimum)
		{
			m_timeMinimum = localTimeMinimum;

			return &other;
		}

		return m_owner;
	}

}; // namespace Tmpl