#pragma once

#include "base/Main.hpp"

#include "voxels/oop/Ray.hpp"

namespace Tmpl {

	class Voxel
	{

	public:

		Voxel();

		void setup(
			const glm::vec3& position, float halfSize,
			const glm::vec3& color);

		bool isCulled() const { return m_culled; }
		void setCulled(bool value) { m_culled = value; }

		const glm::vec3& getPosition() const { return m_position; }

		const glm::vec3& getAABBMinimum() const { return m_aabbMinimum; }
		const glm::vec3& getAABBMaximum() const { return m_aabbMaximum; }

		const glm::vec3& getColor() const { return m_color; }

	private:

		glm::vec3 m_position;
		glm::vec3 m_aabbMinimum;
		glm::vec3 m_aabbMaximum;
		glm::vec3 m_color;
		bool m_culled;

	};

}; // namespace Tmpl