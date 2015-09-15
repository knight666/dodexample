#pragma once

#include "base/Main.hpp"
#include "voxels/Options.hpp"
#include "voxels/Renderer.hpp"

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

	public:

		Logic(float halfSize);
		virtual ~Logic();

		virtual bool initialize(
			const std::vector<VoxelData>& voxels) = 0;

		virtual size_t cullVoxels(
			const Options& options,
			const glm::vec3& targetPosition) = 0;

		virtual size_t render(
			const Options& options,
			Renderer::Vertex* target) = 0;

	protected:

		float m_voxelHalfSize;

	};

}; // namespace Tmpl