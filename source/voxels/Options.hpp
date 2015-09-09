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
		{
		}

		CameraType camera;
		bool culling;
		bool help;
	};

}; // namespace Tmpl