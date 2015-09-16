#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Voxel;

	class Ray
	{

	public:

		Ray();

		void setup(
			Voxel& owner,
			const glm::vec3& origin,
			const glm::vec3& direction);

		Voxel* getClosest() const { return m_closest; }

		Voxel* intersects(Voxel& other);

	private:

		Voxel* m_owner;
		Voxel* m_closest;
		glm::vec3 m_origin;
		glm::vec3 m_direction;
		float m_timeMinimum;

	};

}; // namespace Tmpl