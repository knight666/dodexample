#include "helpers-streams.hpp"

#include "helpers-strings.hpp"

namespace helpers {

	StreamState createStream(const std::string& text)
	{
		StreamState stream;
		memset(&stream, 0, sizeof(StreamState));

		std::vector<unicode_t> converted = helpers::utf32(text);

		for (std::vector<unicode_t>::iterator it = converted.begin(); it != converted.end(); ++it)
		{
			stream.codepoint[stream.current] = *it;
			stream.quick_check[stream.current] = QuickCheckResult_Yes;
			stream.canonical_combining_class[stream.current] = database_queryproperty(*it, UnicodeProperty_CanonicalCombiningClass);
			stream.current++;
		}

		return stream;
	}

	std::string quickCheckToString(uint8_t quickCheck)
	{
		switch (quickCheck)
		{

		case QuickCheckResult_Yes:
			return "Yes";

		case QuickCheckResult_Maybe:
			return "Maybe";

		case QuickCheckResult_No:
			return "No";

		default:
			std::stringstream ss;
			ss << "<invalid> (0x";
			ss << std::hex << std::setfill('0') << std::setw(2) << std::uppercase << (int)quickCheck;
			ss << ")";
			return ss.str();

		}
	}

	::testing::AssertionResult CompareStream(
		const char* expressionExpected GTEST_ATTRIBUTE_UNUSED_, const char* expressionActual GTEST_ATTRIBUTE_UNUSED_,
		const StreamEntry& entryExpected, const StreamEntry& entryActual)
	{
		if (entryActual.codepoint == entryExpected.codepoint &&
			entryActual.quick_check == entryExpected.quick_check &&
			entryActual.canonical_combining_class == entryExpected.canonical_combining_class)
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			::testing::AssertionResult result = ::testing::AssertionFailure();

			result << "Unexpected entry in stream at index " << (int)entryActual.index << std::endl;

			if (entryActual.codepoint != entryExpected.codepoint)
			{
				result << "[Codepoint]" << std::endl;
				result << "  Actual:  " << identifiable(entryActual.codepoint) << " \"" << printable(entryActual.codepoint) << "\"" << std::endl;
				result << "Expected:  " << identifiable(entryExpected.codepoint) << " \"" << printable(entryExpected.codepoint) << "\"" << std::endl;
			}
			else
			{
				result << "[Codepoint]  " << identifiable(entryActual.codepoint) << " \"" << printable(entryActual.codepoint) << "\"" << std::endl;
			}

			if (entryActual.quick_check != entryExpected.quick_check)
			{
				result << "[QuickCheck]" << std::endl;
				result << "  Actual:  " << quickCheckToString(entryActual.quick_check) << std::endl;
				result << "Expected:  " << quickCheckToString(entryExpected.quick_check) << std::endl;
			}
			else
			{
				result << "[QuickCheck]  " << quickCheckToString(entryActual.quick_check) << std::endl;
			}

			if (entryActual.canonical_combining_class != entryExpected.canonical_combining_class)
			{
				result << "[CanonicalCombiningClass]" << std::endl;
				result << "  Actual:  " << (int)entryActual.canonical_combining_class << std::endl;
				result << "Expected:  " << (int)entryExpected.canonical_combining_class;
			}
			else
			{
				result << "[CanonicalCombiningClass]  " << (int)entryActual.canonical_combining_class;
			}

			return result;
		}
	}

};