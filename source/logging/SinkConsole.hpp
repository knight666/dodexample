#pragma once

#include "base/Main.hpp"

#include "logging/Sink.hpp"

namespace Tmpl {

	class SinkConsole
		: public Sink
	{

	public:

		virtual void write(
			Levels level,
			const char* module,
			const char* timestamp,
			const char* filename, int line,
			const char* message) override;

	}; // class SinkConsole

}; // namespace Tmpl