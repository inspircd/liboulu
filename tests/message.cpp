// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>

#include <oulu/message.hpp>

TEST_CASE("Test that EscapeTag functions as expected")
{
	REQUIRE(Oulu::EscapeTag("foo;bar") == "foo\\:bar");
	REQUIRE(Oulu::EscapeTag("foo bar") == "foo\\sbar");
	REQUIRE(Oulu::EscapeTag("foo\\bar") == "foo\\\\bar");
	REQUIRE(Oulu::EscapeTag("foo\rbar") == "foo\\rbar");
	REQUIRE(Oulu::EscapeTag("foo\nbar") == "foo\\nbar");
}

TEST_CASE("Test that IsCTCP functions as expected")
{
	SECTION("Test that we can handle valid CTCPs")
	{
		REQUIRE(Oulu::IsCTCP("\1FOO\1"));
		REQUIRE(Oulu::IsCTCP("\1FOO bar\1"));
	}

	SECTION("Test that we can handle weird CTCPs that exist in the wild")
	{
		REQUIRE(Oulu::IsCTCP("\1FOO"));
		REQUIRE(Oulu::IsCTCP("\1FOO "));
		REQUIRE(Oulu::IsCTCP("\1FOO \1"));
		REQUIRE(Oulu::IsCTCP("\1FOO bar"));
	}

	SECTION("Test that we reject malformed CTCPs")
	{
		REQUIRE(!Oulu::IsCTCP("FOO"));
		REQUIRE(!Oulu::IsCTCP("\1"));
		REQUIRE(!Oulu::IsCTCP("\1\1"));
		REQUIRE(!Oulu::IsCTCP("\1 FOO"));
	}
}

TEST_CASE("Test that ParseCTCP(name) functions as expected")
{
	std::string_view name;

	SECTION("Test that we can handle valid CTCPs")
	{
		REQUIRE(Oulu::ParseCTCP("\1FOO\1", name));
		REQUIRE(name == "FOO");

		REQUIRE(Oulu::ParseCTCP("\1FOO bar\1", name));
		REQUIRE(name == "FOO");
	}

	SECTION("Test that we can handle weird CTCPs that exist in the wild")
	{
		REQUIRE(Oulu::ParseCTCP("\1FOO", name));
		REQUIRE(name == "FOO");

		REQUIRE(Oulu::ParseCTCP("\1FOO ", name));
		REQUIRE(name == "FOO");

		REQUIRE(Oulu::ParseCTCP("\1FOO \1", name));
		REQUIRE(name == "FOO");

		REQUIRE(Oulu::ParseCTCP("\1FOO bar", name));
		REQUIRE(name == "FOO");
	}

	SECTION("Test that we reject malformed CTCPs")
	{
		REQUIRE(!Oulu::ParseCTCP("FOO", name));
		REQUIRE(name.empty());

		REQUIRE(!Oulu::ParseCTCP("\1", name));
		REQUIRE(name.empty());

		REQUIRE(!Oulu::ParseCTCP("\1\1", name));
		REQUIRE(name.empty());

		REQUIRE(!Oulu::ParseCTCP("\1 FOO", name));
		REQUIRE(name.empty());
	}
}

TEST_CASE("Test that ParseCTCP(name, body) functions as expected")
{
	std::string_view name;
	std::string_view body;

	SECTION("Test that we can handle valid CTCPs")
	{
		REQUIRE(Oulu::ParseCTCP("\1FOO\1", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body.empty());

		REQUIRE(Oulu::ParseCTCP("\1FOO bar\1", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body == "bar");
	}

	SECTION("Test that we can handle weird CTCPs that exist in the wild")
	{
		REQUIRE(Oulu::ParseCTCP("\1FOO", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body.empty());

		REQUIRE(Oulu::ParseCTCP("\1FOO ", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body.empty());

		REQUIRE(Oulu::ParseCTCP("\1FOO \1", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body.empty());

		REQUIRE(Oulu::ParseCTCP("\1FOO bar", name, body));
		REQUIRE(name == "FOO");
		REQUIRE(body == "bar");
	}

	SECTION("Test that we reject malformed CTCPs")
	{
		REQUIRE(!Oulu::ParseCTCP("FOO", name, body));
		REQUIRE(name.empty());
		REQUIRE(body.empty());

		REQUIRE(!Oulu::ParseCTCP("\1", name, body));
		REQUIRE(name.empty());
		REQUIRE(body.empty());

		REQUIRE(!Oulu::ParseCTCP("\1\1", name, body));
		REQUIRE(name.empty());
		REQUIRE(body.empty());

		REQUIRE(!Oulu::ParseCTCP("\1 FOO", name, body));
		REQUIRE(name.empty());
		REQUIRE(body.empty());
	}
}

TEST_CASE("Test that UnescapeTag functions as expected")
{
	REQUIRE(Oulu::UnescapeTag("foo\\:bar") == "foo;bar");
	REQUIRE(Oulu::UnescapeTag("foo\\sbar") == "foo bar");
	REQUIRE(Oulu::UnescapeTag("foo\\\\bar") == "foo\\bar");
	REQUIRE(Oulu::UnescapeTag("foo\\rbar") == "foo\rbar");
	REQUIRE(Oulu::UnescapeTag("foo\\nbar") == "foo\nbar");
	REQUIRE(Oulu::UnescapeTag("foobar\\") == "foobar");
}

TEST_CASE("Test that MessageTokenizer functions as expected")
{
	const auto* message = "this is :a test";

	SECTION("Test that GetMiddle functions as expected")
	{
		Oulu::MessageTokenizer tokenizer(message);
		std::string_view sv;

		REQUIRE(tokenizer.GetMiddle(sv));
		REQUIRE(sv == "this");

		REQUIRE(tokenizer.GetMiddle(sv));
		REQUIRE(sv == "is");

		REQUIRE(tokenizer.GetMiddle(sv));
		REQUIRE(sv == ":a");

		REQUIRE(tokenizer.GetMiddle(sv));
		REQUIRE(sv == "test");

		REQUIRE(!tokenizer.GetMiddle(sv));
		REQUIRE(sv.empty());
	}

	SECTION("Test that GetTrailing functions as expected")
	{
		Oulu::MessageTokenizer tokenizer(message);
		std::string_view sv;

		REQUIRE(tokenizer.GetTrailing(sv));
		REQUIRE(sv == "this");

		REQUIRE(tokenizer.GetTrailing(sv));
		REQUIRE(sv == "is");

		REQUIRE(tokenizer.GetTrailing(sv));
		REQUIRE(sv == "a test");

		REQUIRE(!tokenizer.GetTrailing(sv));
		REQUIRE(sv.empty());
	}
}
