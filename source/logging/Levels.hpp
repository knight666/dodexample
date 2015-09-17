#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	enum class Levels
	{
		Trace,
		Information,
		Warning,
		Error,
		Fatal
	};

	inline const char* levelToString(Levels level)
	{
		static const char* LevelToStringMap[] = {
			"TRACE",
			"INFO",
			"WARN",
			"ERROR",
			"FATAL"
		};
		return LevelToStringMap[(size_t)level];
	}

}; // namespace Tmpl