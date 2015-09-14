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

		struct Vertex
		{
			enum class Offset
			{
				Position = 0,
				Color = sizeof(glm::vec3)
			};

			glm::vec3 position;
			glm::vec3 color;
		};

		struct Uniforms
		{
			glm::mat4x4 modelViewProjection;
		};

	public:

		LogicDOD();
		~LogicDOD();

		virtual bool initialize() override;

		virtual void setVoxels(
			const std::vector<VoxelData>& voxels,
			float halfSize) override;

		virtual size_t cullVoxels(
			const Options& options,
			const glm::vec3& targetPosition) override;

		virtual void render(
			const Options& options,
			const glm::mat4x4& modelViewProjection) override;

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
			size_t ray_closest[Logic::MaxVoxelCount];
		};
		VoxelCollection m_collection;
		size_t m_collectionActive;

		float m_voxelHalfSize;

	};

}; // namespace Tmpl