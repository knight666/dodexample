#pragma once

#include "base/Main.hpp"

#include "voxels/oop/Voxel.hpp"
#include "voxels/Logic.hpp"

namespace Tmpl {

	class LogicOOP
		: public Logic
	{

	public:

		LogicOOP();
		~LogicOOP();

		virtual void generateVoxels(size_t count, float halfSize) override;

		virtual void update(uint32_t milliSeconds) override;
		virtual void render() override;

	private:

		std::vector<Voxel> m_voxels;
		size_t m_voxelsActive;
		float m_voxelHalfSize;

	};

}; // namespace Tmpl