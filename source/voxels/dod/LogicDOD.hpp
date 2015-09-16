#pragma once

#include "base/Main.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	class LogicDOD
		: public Logic
	{

	public:

		LogicDOD();
		~LogicDOD();

		virtual bool initialize(
			const Options& options,
			const std::vector<VoxelData>& voxels) override;

		virtual size_t cullVoxels(
			const Options& options,
			const glm::vec3& targetPosition) override;

		virtual size_t render(
			const Options& options,
			Renderer::Vertex* target) override;

	private:

		struct VoxelCollection
		{
			float voxel_position_x[Options::MaxVoxelCount];
			float voxel_position_y[Options::MaxVoxelCount];
			float voxel_position_z[Options::MaxVoxelCount];
			float voxel_color_r[Options::MaxVoxelCount];
			float voxel_color_g[Options::MaxVoxelCount];
			float voxel_color_b[Options::MaxVoxelCount];
			bool voxel_culled[Options::MaxVoxelCount];
			float ray_direction_x[Options::MaxVoxelCount];
			float ray_direction_y[Options::MaxVoxelCount];
			float ray_direction_z[Options::MaxVoxelCount];
			float ray_time_minimum[Options::MaxVoxelCount];
			size_t ray_closest[Options::MaxVoxelCount];
		};
		VoxelCollection m_collection;
		size_t m_collectionActive;

	};

}; // namespace Tmpl