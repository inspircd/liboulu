// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <cxxtest/TestSuite.h>

#include <oulu/message.hpp>

class TestMessageTokenizer : public CxxTest::TestSuite
{
public:
	void TestGetMiddle()
	{
		const auto* message = "this is :a test";

		Oulu::MessageTokenizer tokenizer(message);
		std::string_view sv;

		TS_ASSERT(tokenizer.GetMiddle(sv))
		TS_ASSERT_EQUALS(sv, "this")

		TS_ASSERT(tokenizer.GetMiddle(sv))
		TS_ASSERT_EQUALS(sv, "is")

		TS_ASSERT(tokenizer.GetMiddle(sv))
		TS_ASSERT_EQUALS(sv, ":a")

		TS_ASSERT(tokenizer.GetMiddle(sv))
		TS_ASSERT_EQUALS(sv, "test")

		TS_ASSERT(!tokenizer.GetMiddle(sv))
		TS_ASSERT_EQUALS(sv, "")
	}

	void TestGetTrailing()
	{
		const auto* message = "this is :a test";

		Oulu::MessageTokenizer tokenizer(message);
		std::string_view sv;

		TS_ASSERT(tokenizer.GetTrailing(sv))
		TS_ASSERT_EQUALS(sv, "this")

		TS_ASSERT(tokenizer.GetTrailing(sv))
		TS_ASSERT_EQUALS(sv, "is")

		TS_ASSERT(tokenizer.GetTrailing(sv))
		TS_ASSERT_EQUALS(sv, "a test")

		TS_ASSERT(!tokenizer.GetTrailing(sv))
		TS_ASSERT_EQUALS(sv, "")
	}
};
