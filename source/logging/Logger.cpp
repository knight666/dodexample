#include "logging/Logger.hpp"

#include "logging/SinkConsole.hpp"
#include "logging/SinkFile.hpp"

namespace Tmpl {

	Logger* Logger::s_instance = nullptr;

	Logger::Logger()
	{
		addSink(SinkPtr(new SinkConsole()));
		addSink(SinkPtr(new SinkFile("example.log")));
	}

	Logger::~Logger()
	{
		m_sinks.clear();
	}

	Logger& Logger::get()
	{
		return *s_instance;
	}

	void Logger::initialize()
	{
		s_instance = new Logger();
	}

	void Logger::destroy()
	{
		delete s_instance;
		s_instance = nullptr;
	}

	void Logger::addSink(SinkPtr a_Sink)
	{
		m_sinks.push_back(a_Sink);
	}

	void Logger::write(Levels level, const char* module, const char* filepath, int line, const char* message)
	{
		std::unique_lock<std::mutex>(m_lock);

		time_t current_time = time(0);
		struct tm* now = localtime(&current_time);

		char timestamp[32] = { 0 };
		_snprintf(
			timestamp,
			31,
			"%02d:%02d:%02d",
			now->tm_hour,
			now->tm_min,
			now->tm_sec);

		const char* path_last_slash = strrchr(filepath, '\\');
		const char* filename = (path_last_slash != nullptr) ? (path_last_slash + 1) : filepath;

		for (SinkPtr& sink : m_sinks)
		{
			sink->write(level, module, timestamp, filename, line, message);
		}
	}

}; // namespace Tmpl