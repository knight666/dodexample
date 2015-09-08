#pragma once

#include "base/Main.hpp"

#include "logging/Context.hpp"
#include "logging/Sink.hpp"

#define TMPL_LOG_INFO(_module)   Tmpl::Context(Tmpl::Levels::Information, #_module, __FILE__, __LINE__)
#define TMPL_LOG_TRACE(_module)  Tmpl::Context(Tmpl::Levels::Trace, #_module, __FILE__, __LINE__)
#define TMPL_LOG_WARN(_module)   Tmpl::Context(Tmpl::Levels::Warning, #_module, __FILE__, __LINE__)
#define TMPL_LOG_ERROR(_module)  Tmpl::Context(Tmpl::Levels::Error, #_module, __FILE__, __LINE__)
#define TMPL_LOG_FATAL(_module)  Tmpl::Context(Tmpl::Levels::Fatal, #_module, __FILE__, __LINE__)

namespace Tmpl {

	class Logger
	{

	public:

		static Logger& get();

		static void initialize();
		static void destroy();

		void addSink(SinkPtr sink);

		void write(Levels level, const char* module, const char* filepath, int line, const char* message);

	private:

		Logger();
		~Logger();

	private:

		static Logger* s_instance;

	private:

		SinkList m_sinks;
		std::mutex m_lock;

	};

}; // namespace Tmpl