#include "logging/Context.hpp"

#include "logging/Logger.hpp"

namespace Tmpl {

	Context::Context(Levels level, const char* module, const char* file, int line)
		: m_level(level)
		, m_module(module)
		, m_file(file)
		, m_line(line)
	{
	}

	Context::~Context()
	{
		std::string message = str();
		message.append("\n");

		Logger::get().write(
			m_level,
			m_module.c_str(),
			m_file.c_str(),
			m_line,
			message.c_str());
	}

}; // namespace Tmpl