#pragma once

#include "base/Main.hpp"

namespace Tmpl {

	// String conversion

	std::string narrow(const wchar_t* wideText, size_t sizeInBytes);
	inline std::string narrow(const wchar_t* wideText)
	{
		return narrow(wideText, wcslen(wideText) * sizeof(wchar_t));
	}
	inline std::string narrow(const std::wstring& wideText)
	{
		return narrow(wideText.c_str(), wideText.length() * sizeof(wchar_t));
	}

	std::wstring widen(const char* utf8Text, size_t sizeInBytes);
	inline std::wstring widen(const char* utf8Text)
	{
		return widen(utf8Text, strlen(utf8Text));
	}
	inline std::wstring widen(const std::string& utf8Text)
	{
		return widen(utf8Text.c_str(), utf8Text.length() * sizeof(wchar_t));
	}

	// GLFW

	void glfwErrors(int errorCode, const char* description);

}; // namespace Tmpl