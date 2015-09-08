#include "base/Utils.hpp"

namespace Tmpl {

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

	void glfwErrors(int errorCode, const char* description)
	{
		TMPL_LOG_ERROR(GLFW) << std::hex << errorCode << ": " << description;
	}

}; // namespace Tmpl