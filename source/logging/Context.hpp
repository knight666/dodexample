#pragma once

#include "base/Main.hpp"
#include "logging/Levels.hpp"

namespace Tmpl {

	class Context
		: public std::ostringstream
	{

	public:

		Context(Levels level, const char* module, const char* file, int line);
		~Context();

	private:

		Levels m_level;
		std::string m_module;
		std::string m_file;
		int m_line;

	};

}; // namespace Tmpl