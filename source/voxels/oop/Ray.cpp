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
		float t0, t1;
		float tmin = 0.f;
		float tmax = std::numeric_limits<float>::max();

		t0 = (other.getAABBMinimum().x - m_origin.x) * m_direction.x;
		t1 = (other.getAABBMaximum().x - m_origin.x) * m_direction.x;
		if (t0 > t1)
		{
			tmin = (t1 > tmin) ? t1 : tmin;
			tmax = (t0 < tmax) ? t0 : tmax;
		}
		else
		{
			tmin = (t0 > tmin) ? t0 : tmin;
			tmax = (t1 < tmax) ? t1 : tmax;
		}

		t0 = (other.getAABBMinimum().y - m_origin.y) * m_direction.y;
		t1 = (other.getAABBMaximum().y - m_origin.y) * m_direction.y;
		if (t0 > t1)
		{
			tmin = (t1 > tmin) ? t1 : tmin;
			tmax = (t0 < tmax) ? t0 : tmax;
		}
		else
		{
			tmin = (t0 > tmin) ? t0 : tmin;
			tmax = (t1 < tmax) ? t1 : tmax;
		}

		t0 = (other.getAABBMinimum().z - m_origin.z) * m_direction.z;
		t1 = (other.getAABBMaximum().z - m_origin.z) * m_direction.z;
		if (t0 > t1)
		{
			tmin = (t1 > tmin) ? t1 : tmin;
			tmax = (t0 < tmax) ? t0 : tmax;
		}
		else
		{
			tmin = (t0 > tmin) ? t0 : tmin;
			tmax = (t1 < tmax) ? t1 : tmax;
		}

		if (tmin <= tmax && tmin <= m_timeMinimum)
		{
			m_timeMinimum = tmin;
			m_closest = &other;

			return true;
		}

		return false;

		/*glm::vec3 localMinimum = (other.getAABBMinimum() - m_origin) * m_direction;
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

		return false;*/
	}

}; // namespace Tmpl