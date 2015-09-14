#pragma once

#include "base/Main.hpp"
#include "voxels/Options.hpp"

namespace Tmpl {

	class Logic
	{

	public:

		struct VoxelData
		{
			glm::vec3 position;
			glm::vec3 color;
		};

		static const size_t MaxVoxelCount = 1024 * 1024;

		virtual ~Logic() { }

		virtual bool initialize() = 0;

		virtual void setVoxels(
			const std::vector<VoxelData>& voxels,
			float halfSize) = 0;

		virtual size_t cullVoxels(const Options& options, const glm::vec3& targetPosition) = 0;
		virtual void render(const Options& options, const glm::mat4x4& modelViewProjection) = 0;

	};

}; // namespace Tmpl