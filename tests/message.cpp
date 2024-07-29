// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <cxxtest/TestSuite.h>

#include <oulu/message.hpp>

class TestFunctions : public CxxTest::TestSuite
{
public:
	void TestEscapeTag()
	{
		TS_ASSERT_EQUALS(Oulu::EscapeTag("foo;bar"), "foo\\:bar")
		TS_ASSERT_EQUALS(Oulu::EscapeTag("foo bar"), "foo\\sbar")
		TS_ASSERT_EQUALS(Oulu::EscapeTag("foo\\bar"), "foo\\\\bar")
		TS_ASSERT_EQUALS(Oulu::EscapeTag("foo\rbar"), "foo\\rbar")
		TS_ASSERT_EQUALS(Oulu::EscapeTag("foo\nbar"), "foo\\nbar")
	}

	void TestIsCTCP()
	{
		// Test that we can handle valid CTCPs.
		TS_ASSERT(Oulu::IsCTCP("\1FOO\1"))
		TS_ASSERT(Oulu::IsCTCP("\1FOO bar\1"))

		// Test that we can handle weird CTCPs that exist in the wild.
		TS_ASSERT(Oulu::IsCTCP("\1FOO"))
		TS_ASSERT(Oulu::IsCTCP("\1FOO "))
		TS_ASSERT(Oulu::IsCTCP("\1FOO \1"))
		TS_ASSERT(Oulu::IsCTCP("\1FOO bar"))

		// Test that we reject malformed CTCPs.
		TS_ASSERT(!Oulu::IsCTCP("FOO"))
		TS_ASSERT(!Oulu::IsCTCP("\1"))
		TS_ASSERT(!Oulu::IsCTCP("\1\1"))
		TS_ASSERT(!Oulu::IsCTCP("\1 FOO"))
	}

	void TestParseCTCP2()
	{
		std::string_view name;

		// Test that we can handle valid CTCPs.
		TS_ASSERT(Oulu::ParseCTCP("\1FOO\1", name))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO bar\1", name))
		TS_ASSERT_EQUALS(name, "FOO")

		// Test that we can handle weird CTCPs that exist in the wild.
		TS_ASSERT(Oulu::ParseCTCP("\1FOO", name))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO ", name))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO \1", name))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO bar", name))
		TS_ASSERT_EQUALS(name, "FOO")

		// Test that we reject malformed CTCPs.
		TS_ASSERT(!Oulu::ParseCTCP("FOO", name))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT(!Oulu::ParseCTCP("\1", name))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT(!Oulu::ParseCTCP("\1\1", name))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT(!Oulu::ParseCTCP("\1 FOO", name))
		TS_ASSERT_EQUALS(name, "");
	}

	void TestParseCTCP3()
	{
		std::string_view name;
		std::string_view body;

		// Test that we can handle valid CTCPs.
		TS_ASSERT(Oulu::ParseCTCP("\1FOO\1", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "");
		TS_ASSERT(Oulu::ParseCTCP("\1FOO bar\1", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "bar");

		// Test that we can handle weird CTCPs that exist in the wild.
		TS_ASSERT(Oulu::ParseCTCP("\1FOO", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO ", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO \1", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(Oulu::ParseCTCP("\1FOO bar", name, body))
		TS_ASSERT_EQUALS(name, "FOO")
		TS_ASSERT_EQUALS(body, "bar")

		// Test that we reject malformed CTCPs.
		TS_ASSERT(!Oulu::ParseCTCP("FOO", name, body))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(!Oulu::ParseCTCP("\1", name, body))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(!Oulu::ParseCTCP("\1\1", name, body))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT_EQUALS(body, "")
		TS_ASSERT(!Oulu::ParseCTCP("\1 FOO", name, body))
		TS_ASSERT_EQUALS(name, "");
		TS_ASSERT_EQUALS(body, "")
	}

	void TestUnescapeTag()
	{
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foo\\:bar"), "foo;bar")
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foo\\sbar"), "foo bar")
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foo\\\\bar"), "foo\\bar")
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foo\\rbar"), "foo\rbar")
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foo\\nbar"), "foo\nbar")
		TS_ASSERT_EQUALS(Oulu::UnescapeTag("foobar\\"), "foobar")
	}
};

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
