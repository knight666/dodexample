#pragma once

#include "base/Main.hpp"
#include "logging/Sink.hpp"

namespace Tmpl {

	class SinkFile
		: public Sink
	{

	public:

		SinkFile(const std::string& filename);

		virtual void write(Levels level, const char* module, const char* timestamp, const char* filename, int line, const char* message) override;

	private:

		std::string m_filePath;
		std::ofstream m_file;

	};

}; // namespace Tmpl