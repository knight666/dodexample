#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Voxel;

	class Ray
	{

	public:

		Ray(Voxel& owner, const glm::vec3& origin, const glm::vec3& direction);
		~Ray();

		bool intersects(Voxel& other);

	private:

		Voxel& m_owner;
		glm::vec3 m_origin;
		glm::vec3 m_direction;
		float m_timeMinimum;
		float m_timeMaximum;

	};

}; // namespace Tmpl