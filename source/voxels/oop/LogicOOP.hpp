#pragma once

#include "base/Main.hpp"

#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/VertexArrays.hpp"
#include "voxels/oop/Voxel.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	class LogicOOP
		: public Logic
	{

	public:

		LogicOOP(float halfSize);
		~LogicOOP();

		virtual bool initialize(
			const std::vector<VoxelData>& voxels) override;

		virtual size_t cullVoxels(
			const Options& options,
			const glm::vec3& targetPosition) override;

		virtual size_t render(
			const Options& options,
			Vertex* target) override;

	private:

		std::vector<Voxel> m_voxels;
		std::vector<Ray> m_rays;
		size_t m_voxelsActive;

		Vertex m_vertexData[Logic::MaxVoxelCount];

	};

}; // namespace Tmpl