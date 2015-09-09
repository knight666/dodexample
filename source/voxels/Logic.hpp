#pragma once

namespace Tmpl {

	class Logic
	{

	public:

		static const size_t MaxVoxelCount = 1024 * 1024;

		virtual ~Logic() { }

		virtual bool initialize() = 0;

		virtual void generateVoxels(size_t count, float halfSize) = 0;

		virtual void update(uint32_t milliSeconds) = 0;
		virtual void render() = 0;

	};

}; // namespace Tmpl