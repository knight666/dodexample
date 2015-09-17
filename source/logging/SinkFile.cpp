#include "logging/SinkFile.hpp"

namespace Tmpl {

	SinkFile::SinkFile(const std::string& filename)
	{
		m_filePath = applicationDirectory() + filename;
		
		m_file.open(m_filePath.c_str(), std::ios::out | std::ios::trunc);
		if (m_file.is_open())
		{
			m_file.close();
		}
	}

	void SinkFile::write(Levels level, const char* module, const char* timestamp, const char* filename, int line, const char* message)
	{
		m_file.open(m_filePath.c_str(), std::ios::out | std::ios::app);
		if (!m_file.is_open())
		{
			return;
		}

		char logmessage_formatted[1024] = { 0 };

		if (level >= Levels::Warning)
		{
			_snprintf(
				logmessage_formatted,
				1023,
				"(%s:%d) (%s) [%s] [%s] ",
				filename,
				line,
				timestamp,
				levelToString(level),
				module);
		}
		else
		{
			_snprintf(
				logmessage_formatted,
				1023,
				"(%s) [%s] [%s] ",
				timestamp,
				levelToString(level),
				module);
		}
		
		m_file << logmessage_formatted;
		m_file << message;

		m_file.close();
	}

}; // namespace Tmpl