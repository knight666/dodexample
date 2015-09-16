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

		enum class Scene
		{
			Small,
			Medium,
			Large
		};

		static const size_t MaxVoxelCount = 1024 * 1024;

		Options()
			: logic(LogicType::ObjectOriented)
			, camera(CameraType::User)
			, scene(Scene::Small)
			, lightRunning(true)
			, culling(true)
			, help(false)
			, showCulled(false)
			, voxelHalfSize(20.0f)
			, profiling(true)
			, profilingFrames(1000)
		{
		}

		LogicType logic;
		CameraType camera;
		Scene scene;
		bool lightRunning;
		bool culling;
		bool help;
		bool showCulled;
		float voxelHalfSize;
		bool profiling;
		size_t profilingFrames;

	};

}; // namespace Tmpl