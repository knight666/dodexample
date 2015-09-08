#include "logging/SinkConsole.hpp"

namespace Tmpl {

	void SinkConsole::write(
		Levels level,
		const char* module,
		const char* timestamp,
		const char* filename, int line,
		const char* message)
	{
		char logmessage_formatted[1024] = { 0 };

		_snprintf(
			logmessage_formatted,
			1023,
			"(%s) [%s] [%s] %s",
			timestamp,
			levelToString(level),
			module,
			message);

		fprintf(
			(level >= Levels::Warning) ? stderr : stdout,
			logmessage_formatted);

	#if _WINDOWS
		::OutputDebugStringA(logmessage_formatted);
	#endif
	}

}; // namespace Tmpl