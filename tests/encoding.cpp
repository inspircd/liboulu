// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <catch2/catch_test_macros.hpp>

#include <oulu/encoding.hpp>

TEST_CASE("Test that Base64Decode functions as expected")
{
	SECTION("Test that we can handle regular decoding")
	{
		REQUIRE(Oulu::Base64Decode("") == "");
		REQUIRE(Oulu::Base64Decode("Zg==") == "f");
		REQUIRE(Oulu::Base64Decode("Zm8=") == "fo");
		REQUIRE(Oulu::Base64Decode("Zm9v") == "foo");
	}

	SECTION("Test that we can handle no padding")
	{
		REQUIRE(Oulu::Base64Decode("Zg") == "f");
		REQUIRE(Oulu::Base64Decode("Zm8") == "fo");
	}

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::Base64Decode("Zm9vYmFy", 4) == "foo");
	}

	SECTION("Test that we can handle alternate Base64 tables")
	{
		REQUIRE(Oulu::Base64Decode("fn5-", Oulu::BASE64_URL_TABLE) == "~~~");
	}
}

TEST_CASE("Test that Base64Encode functions as expected")
{
	SECTION("Test that we can handle regular encoding")
	{
		REQUIRE(Oulu::Base64Encode("") == "");
		REQUIRE(Oulu::Base64Encode("f") == "Zg==");
		REQUIRE(Oulu::Base64Encode("fo") == "Zm8=");
		REQUIRE(Oulu::Base64Encode("foo") == "Zm9v");
	}

	SECTION("Test that that we can handle no padding")
	{
		REQUIRE(Oulu::Base64Encode("f", nullptr, 0) == "Zg");
		REQUIRE(Oulu::Base64Encode("fo", nullptr, 0) == "Zm8");
	}

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::Base64Encode("foobar", 3) == "Zm9v");
	}

	SECTION("Test that we can handle alternate Base64 tables")
	{
		REQUIRE(Oulu::Base64Encode("~~~", Oulu::BASE64_URL_TABLE) == "fn5-");
	}
}

TEST_CASE("Test that HexDecode functions as expected")
{
	SECTION("Test that we can decode using the default table")
	{
		REQUIRE(Oulu::HexDecode("") == "");
		REQUIRE(Oulu::HexDecode("66") == "f");
		REQUIRE(Oulu::HexDecode("666f") == "fo");
		REQUIRE(Oulu::HexDecode("666f6f") == "foo");
	}

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::HexDecode("666f6f666f6f", 6) == "foo");
	}

	SECTION("Test that we can decode using the upper-case table")
	{
		REQUIRE(Oulu::HexDecode("66", Oulu::HEX_TABLE_UPPER) == "f");
		REQUIRE(Oulu::HexDecode("666F", Oulu::HEX_TABLE_UPPER) == "fo");
		REQUIRE(Oulu::HexDecode("666F6F", Oulu::HEX_TABLE_UPPER) == "foo");
	}

	SECTION("Test that we can decode using separators")
	{
		REQUIRE(Oulu::HexDecode("66", nullptr, ':') == "f");
		REQUIRE(Oulu::HexDecode("66:6f", nullptr, ':') == "fo");
		REQUIRE(Oulu::HexDecode("66:6f:6f", nullptr, ':') == "foo");
	}
}

TEST_CASE("Test that HexEncode functions as expected")
{
	SECTION("Test that we can encode using the default table")
	{
		REQUIRE(Oulu::HexEncode("") == "");
		REQUIRE(Oulu::HexEncode("f") == "66");
		REQUIRE(Oulu::HexEncode("fo") == "666f");
		REQUIRE(Oulu::HexEncode("foo") == "666f6f");
	}

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::HexEncode("foobar", 3) == "666f6f");
	}

	SECTION("Test that we can encode using the upper-case table")
	{
		REQUIRE(Oulu::HexEncode("f", Oulu::HEX_TABLE_UPPER) == "66");
		REQUIRE(Oulu::HexEncode("fo", Oulu::HEX_TABLE_UPPER) == "666F");
		REQUIRE(Oulu::HexEncode("foo", Oulu::HEX_TABLE_UPPER) == "666F6F");
	}

	SECTION("Test that we can encode using separators")
	{
		REQUIRE(Oulu::HexEncode("f", nullptr, ':') == "66");
		REQUIRE(Oulu::HexEncode("fo", nullptr, ':') == "66:6f");
		REQUIRE(Oulu::HexEncode("foo", nullptr, ':') == "66:6f:6f");
	}
}

TEST_CASE("Test that PercentDecode functions as expected")
{
	SECTION("Test that we can decode using the default table")
	{
		REQUIRE(Oulu::PercentDecode("") == "");
		REQUIRE(Oulu::PercentDecode("foo") == "foo");
		REQUIRE(Oulu::PercentDecode("foo%20bar") == "foo bar");
		REQUIRE(Oulu::PercentDecode("foo%20bar%3F") == "foo bar?");
	};

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::PercentDecode("foo%20bar", 6) == "foo ");
	}
}

TEST_CASE("Test that PercentEncode functions as expected")
{
	SECTION("Test that we can decode using the default table")
	{
		REQUIRE(Oulu::PercentEncode("") == "");
		REQUIRE(Oulu::PercentEncode("foo") == "foo");
		REQUIRE(Oulu::PercentEncode("foo bar") == "foo%20bar");
		REQUIRE(Oulu::PercentEncode("foo bar?") == "foo%20bar%3F");
	}

	SECTION("Test that we can handle truncation")
	{
		REQUIRE(Oulu::PercentEncode("foo bar", 4) == "foo%20");
	}

	SECTION("Test that we can encode using an alternate table")
	{
		static constexpr const auto* table = "bar";
		REQUIRE(Oulu::PercentEncode("foo", table) == "%66%6F%6F");
		REQUIRE(Oulu::PercentEncode("foo bar", table) == "%66%6F%6F%20bar");
	}

	SECTION("Test that we can encode using the lower-case table")
	{
		REQUIRE(Oulu::PercentEncode("foo?", nullptr, false) == "foo%3f");
	}
}
