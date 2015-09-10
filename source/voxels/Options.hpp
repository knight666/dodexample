#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	struct Options
	{
		enum class CameraType
		{
			User,
			Target
		};

		Options()
			: camera(CameraType::User)
			, culling(true)
			, help(false)
			, showCulled(false)
		{
		}

		CameraType camera;
		bool culling;
		bool help;
		bool showCulled;
	};

}; // namespace Tmpl