#pragma once

#include "base/Main.hpp"

#include "voxels/oop/Voxel.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	class Buffer;
	class Program;

	class LogicOOP
		: public Logic
	{

		struct Vertex
		{
			enum class Offset
			{
				Position = 0,
				Color = sizeof(glm::vec4)
			};

			glm::vec4 position;
			glm::vec4 color;
		};

		struct GeometryUniforms
		{
			glm::mat4x4 modelViewProjection;
			float halfSize;
		};

	public:

		LogicOOP();
		~LogicOOP();

		virtual bool initialize() override;

		virtual void generateVoxels(size_t count, float halfSize) override;

		virtual void update(uint32_t milliSeconds) override;
		virtual void render() override;

	private:

		std::vector<Voxel> m_voxels;
		size_t m_voxelsActive;
		float m_voxelHalfSize;

		std::shared_ptr<Program> m_program;
		GLint m_uniformTransform;

	};

}; // namespace Tmpl