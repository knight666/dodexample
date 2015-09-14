#include "voxels/oop/Ray.hpp"

#include "voxels/oop/Voxel.hpp"

namespace Tmpl {

	Ray::Ray()
		: m_owner(nullptr)
		, m_closest(nullptr)
		, m_timeMinimum(std::numeric_limits<float>::max())
		, m_timeMaximum(0.0f)
	{
	}

	void Ray::setup(
		Voxel& owner,
		const glm::vec3& origin,
		const glm::vec3& direction)
	{
		m_owner = &owner;
		m_closest = nullptr;
		m_origin = origin;
		m_direction = direction;
		m_timeMinimum = std::numeric_limits<float>::max();
		m_timeMaximum = 0.0f;
	}

	bool Ray::intersects(Voxel& other)
	{
		glm::vec3 offsetMinimum = (other.getAABBMinimum() - m_origin) * m_direction;
		glm::vec3 offsetMaximum = (other.getAABBMaximum() - m_origin) * m_direction;

		float localTimeMinimum = 0.f;
		float localTimeMaximum = std::numeric_limits<float>::max();

		if (offsetMinimum.x > offsetMaximum.x)
		{
			localTimeMinimum = glm::max(offsetMaximum.x, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMinimum.x, localTimeMaximum);
		}
		else
		{
			localTimeMinimum = glm::max(offsetMinimum.x, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMaximum.x, localTimeMaximum);
		}

		if (offsetMinimum.y > offsetMaximum.y)
		{
			localTimeMinimum = glm::max(offsetMaximum.y, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMinimum.y, localTimeMaximum);
		}
		else
		{
			localTimeMinimum = glm::max(offsetMinimum.y, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMaximum.y, localTimeMaximum);
		}

		if (offsetMinimum.z > offsetMaximum.z)
		{
			localTimeMinimum = glm::max(offsetMaximum.z, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMinimum.z, localTimeMaximum);
		}
		else
		{
			localTimeMinimum = glm::max(offsetMinimum.z, localTimeMinimum);
			localTimeMaximum = glm::min(offsetMaximum.z, localTimeMaximum);
		}

		if (localTimeMinimum <= localTimeMaximum &&
			localTimeMinimum <= m_timeMinimum)
		{
			m_timeMinimum = localTimeMinimum;
			m_closest = &other;

			return true;
		}

		return false;
	}

}; // namespace Tmpl