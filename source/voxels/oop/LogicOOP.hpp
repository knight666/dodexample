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

		LogicOOP();
		~LogicOOP();

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

		std::vector<Voxel> m_voxels;
		std::vector<Ray> m_rays;
		size_t m_voxelsActive;
		float m_voxelHalfSize;

		Vertex m_vertexData[Logic::MaxVoxelCount];

	};

}; // namespace Tmpl