#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	class Logic
	{

	public:

		static const size_t MaxVoxelCount = 1024 * 1024;

		virtual ~Logic() { }

		virtual bool initialize() = 0;

		virtual void generateVoxels(size_t count, float halfSize) = 0;

		virtual void update(uint32_t milliSeconds) = 0;
		virtual void cullVoxels(const glm::vec3& targetPosition) = 0;
		virtual void render(const glm::mat4x4& modelViewProjection) = 0;

	};

}; // namespace Tmpl