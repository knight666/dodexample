#pragma once

#include "base/Main.hpp"

#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/VertexArrays.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	class LogicDOD
		: public Logic
	{

	public:

		LogicDOD(float halfSize);
		~LogicDOD();

		virtual bool initialize(
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
			float voxel_position_x[Logic::MaxVoxelCount];
			float voxel_position_y[Logic::MaxVoxelCount];
			float voxel_position_z[Logic::MaxVoxelCount];
			float voxel_color_r[Logic::MaxVoxelCount];
			float voxel_color_g[Logic::MaxVoxelCount];
			float voxel_color_b[Logic::MaxVoxelCount];
			bool voxel_culled[Logic::MaxVoxelCount];
			float ray_direction_x[Logic::MaxVoxelCount];
			float ray_direction_y[Logic::MaxVoxelCount];
			float ray_direction_z[Logic::MaxVoxelCount];
			float ray_time_minimum[Logic::MaxVoxelCount];
		};
		VoxelCollection m_collection;
		size_t m_collectionActive;

	};

}; // namespace Tmpl