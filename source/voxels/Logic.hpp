#pragma once

#include "base/Main.hpp"
#include "graphics/Buffer.hpp"
#include "graphics/Program.hpp"
#include "graphics/VertexArrays.hpp"
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

	public:

		Logic();
		virtual ~Logic();

		virtual bool initialize() = 0;

		virtual void setVoxels(
			const std::vector<VoxelData>& voxels,
			float halfSize) = 0;

		virtual size_t cullVoxels(
			const Options& options,
			const glm::vec3& targetPosition) = 0;

		virtual void render(
			const Options& options,
			const glm::mat4x4& modelViewProjection) = 0;

	protected:

		std::shared_ptr<Program> m_program;
		std::shared_ptr<Buffer> m_vertices;
		std::shared_ptr<VertexArrays> m_attributes;
		std::shared_ptr<Buffer> m_uniforms;
		GLint m_uniformTransform;
		GLint m_uniformHalfSize;

	};

}; // namespace Tmpl