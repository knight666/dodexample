#pragma once

#include "base/Main.hpp"
#include "logging/Levels.hpp"

namespace Tmpl {

	class Sink
	{

	public:

		virtual ~Sink() { }

		virtual void write(Levels level, const char* module, const char* timestamp, const char* filename, int line, const char* message) = 0;

	};

	typedef std::shared_ptr<Sink> SinkPtr;
	typedef std::vector<SinkPtr> SinkList;

}; // namespace Tmpl