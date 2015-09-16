#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	struct Options
	{

		enum class LogicType
		{
			ObjectOriented,
			DataOriented
		};

		enum class CameraType
		{
			User,
			Target
		};

		Options()
			: logic(LogicType::ObjectOriented)
			, camera(CameraType::User)
			, culling(true)
			, help(false)
			, showCulled(false)
			, voxelHalfSize(20.0f)
		{
		}

		LogicType logic;
		CameraType camera;
		bool culling;
		bool help;
		bool showCulled;
		float voxelHalfSize;
		static const size_t MaxVoxelCount = 1024 * 1024;

	};

}; // namespace Tmpl