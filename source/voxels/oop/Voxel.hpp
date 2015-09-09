#pragma once

#include "base/Main.hpp"

#include "voxels/oop/Ray.hpp"

namespace Tmpl {

	class Voxel
	{

	public:

		void setup(
			const glm::vec3& position, float halfSize,
			const glm::vec4& color);

		const glm::vec3& getPosition() const { return m_position; }

		const glm::vec3& getAABBMinimum() const { return m_aabbMinimum; }
		const glm::vec3& getAABBMaximum() const { return m_aabbMaximum; }

		const glm::vec4& getColor() const { return m_color; }

	private:

		glm::vec3 m_position;
		glm::vec3 m_aabbMinimum;
		glm::vec3 m_aabbMaximum;
		glm::vec4 m_color;

	};

}; // namespace Tmpl