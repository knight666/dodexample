#include "base/Utils.hpp"

namespace Tmpl {

	std::string applicationDirectory()
	{
		std::string target_directory;

	#if _WINDOWS
		wchar_t application_directory[_MAX_PATH] = { 0 };
		if (::SHGetFolderPathW(
			NULL,
			CSIDL_PERSONAL,
			NULL,
			SHGFP_TYPE_CURRENT,
			application_directory) == S_OK)
		{
			target_directory = narrow(application_directory);
		}
	#endif

		target_directory += "/";
		target_directory += TMPL_WINDOW_TITLE;
		target_directory += "/";

	#if _WINDOWS
		std::wstring wide_target_directory = widen(target_directory);

		::CreateDirectoryW(wide_target_directory.c_str(), NULL);

		DWORD directoryAttributes = ::GetFileAttributesW(wide_target_directory.c_str());
		if (directoryAttributes == INVALID_FILE_ATTRIBUTES ||
			(directoryAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			target_directory.clear();
		}
	#endif

		return target_directory;
	}

	std::string narrow(const wchar_t* wideText, size_t sizeInBytes)
	{
		std::string converted;
		int32_t errors = 0;

		size_t convertedSize = widetoutf8(
			wideText, sizeInBytes,
			nullptr, 0,
			&errors);
		if (convertedSize == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(convertedSize);

		widetoutf8(
			wideText, sizeInBytes,
			&converted[0], converted.size(),
			nullptr);

		return converted;
	}

	std::wstring widen(const char* utf8Text, size_t sizeInBytes)
	{
		std::wstring converted;
		int32_t errors = 0;

		size_t convertedSize = utf8towide(
			utf8Text, sizeInBytes,
			nullptr, 0,
			&errors);
		if (convertedSize == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(convertedSize);

		utf8towide(
			utf8Text, sizeInBytes,
			&converted[0], converted.size(),
			nullptr);

		return converted;
	}

	std::string tolower(const char* utf8Text, size_t sizeInBytes)
	{
		std::string converted;
		int32_t errors = 0;

		size_t convertedSize = utf8tolower(
			utf8Text, sizeInBytes,
			nullptr, 0,
			&errors);
		if (convertedSize == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(convertedSize);

		utf8tolower(
			utf8Text, sizeInBytes,
			&converted[0], converted.size(),
			nullptr);

		return converted;
	}

	std::string toupper(const char* utf8Text, size_t sizeInBytes)
	{
		std::string converted;
		int32_t errors = 0;

		size_t convertedSize = utf8toupper(
			utf8Text, sizeInBytes,
			nullptr, 0,
			&errors);
		if (convertedSize == 0 ||
			errors != 0)
		{
			return converted;
		}

		converted.resize(convertedSize);

		utf8toupper(
			utf8Text, sizeInBytes,
			&converted[0], converted.size(),
			nullptr);

		return converted;
	}

	void glfwErrors(int errorCode, const char* description)
	{
		TMPL_LOG_ERROR(GLFW) << std::hex << errorCode << ": " << description;
	}

}; // namespace Tmpl