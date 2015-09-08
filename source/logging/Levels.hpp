#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	enum class Levels
	{
		Information,
		Trace,
		Warning,
		Error,
		Fatal
	};

	inline const char* levelToString(Levels level)
	{
		static const char* LevelToStringMap[] = {
			"INFO",
			"TRACE",
			"WARN",
			"ERROR",
			"FATAL"
		};
		return LevelToStringMap[(size_t)level];
	}

}; // namespace Tmpl