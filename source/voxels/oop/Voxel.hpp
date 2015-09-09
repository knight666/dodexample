#pragma once

#include "base/Main.hpp"

#include "voxels/oop/Ray.hpp"

namespace Tmpl {

	class Voxel
	{

	public:

		Voxel(const glm::vec3& position, float halfSize);
		~Voxel();

		const glm::vec3& getAABBMinimum() const { return m_aabbMinimum; }
		const glm::vec3& getAABBMaximum() const { return m_aabbMaximum; }

	private:

		glm::vec3 m_aabbMinimum;
		glm::vec3 m_aabbMaximum;

	};

}; // namespace Tmpl