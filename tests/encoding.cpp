// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <cxxtest/TestSuite.h>

#include <oulu/encoding.hpp>

class TestFunctions : public CxxTest::TestSuite
{
public:
	void TestBase64Decode()
	{
		// Test that we can handle regular decoding.
		TS_ASSERT_EQUALS(Oulu::Base64Decode(""), "")
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zg=="), "f")
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zm8="), "fo")
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zm9v"), "foo")

		// Test that we can handle no padding
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zg"), "f")
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zm8"), "fo")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::Base64Decode("Zm9vYmFy", 4), "foo")

		// Test that we can handle alternate Base64 tables.
		TS_ASSERT_EQUALS(Oulu::Base64Decode("fn5-", Oulu::BASE64_URL_TABLE), "~~~")
	}

	void TestBase64Encode()
	{
		// Test that we can handle regular encoding.
		TS_ASSERT_EQUALS(Oulu::Base64Encode(""), "")
		TS_ASSERT_EQUALS(Oulu::Base64Encode("f"), "Zg==")
		TS_ASSERT_EQUALS(Oulu::Base64Encode("fo"), "Zm8=")
		TS_ASSERT_EQUALS(Oulu::Base64Encode("foo"), "Zm9v")

		// Test that that we can handle no padding
		TS_ASSERT_EQUALS(Oulu::Base64Encode("f", nullptr, 0), "Zg")
		TS_ASSERT_EQUALS(Oulu::Base64Encode("fo", nullptr, 0), "Zm8")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::Base64Encode("foobar", 3), "Zm9v")

		// Test that we can handle alternate Base64 tables.
		TS_ASSERT_EQUALS(Oulu::Base64Encode("~~~", Oulu::BASE64_URL_TABLE), "fn5-")
	}

	void TestHexDecode()
	{
		// Test that we can decode using the default table.
		TS_ASSERT_EQUALS(Oulu::HexDecode(""), "")
		TS_ASSERT_EQUALS(Oulu::HexDecode("66"), "f")
		TS_ASSERT_EQUALS(Oulu::HexDecode("666f"), "fo")
		TS_ASSERT_EQUALS(Oulu::HexDecode("666f6f"), "foo")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::HexDecode("666f6f666f6f", 6), "foo")

		// Test that we can decode using the upper-case table.
		TS_ASSERT_EQUALS(Oulu::HexDecode("66", Oulu::HEX_TABLE_UPPER), "f")
		TS_ASSERT_EQUALS(Oulu::HexDecode("666F", Oulu::HEX_TABLE_UPPER), "fo")
		TS_ASSERT_EQUALS(Oulu::HexDecode("666F6F", Oulu::HEX_TABLE_UPPER), "foo")

		// Test that we can decode using separators.
		TS_ASSERT_EQUALS(Oulu::HexDecode("66", nullptr, ':'), "f")
		TS_ASSERT_EQUALS(Oulu::HexDecode("66:6f", nullptr, ':'), "fo")
		TS_ASSERT_EQUALS(Oulu::HexDecode("66:6f:6f", nullptr, ':'), "foo")
	}

	void TestHexEncode()
	{
		// Test that we can encode using the default table.
		TS_ASSERT_EQUALS(Oulu::HexEncode(""), "")
		TS_ASSERT_EQUALS(Oulu::HexEncode("f"), "66")
		TS_ASSERT_EQUALS(Oulu::HexEncode("fo"), "666f")
		TS_ASSERT_EQUALS(Oulu::HexEncode("foo"), "666f6f")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::HexEncode("foobar", 3), "666f6f")

		// Test that we can encode using the upper-case table.
		TS_ASSERT_EQUALS(Oulu::HexEncode("f", Oulu::HEX_TABLE_UPPER), "66")
		TS_ASSERT_EQUALS(Oulu::HexEncode("fo", Oulu::HEX_TABLE_UPPER), "666F")
		TS_ASSERT_EQUALS(Oulu::HexEncode("foo", Oulu::HEX_TABLE_UPPER), "666F6F")

		// Test that we can encode using separators.
		TS_ASSERT_EQUALS(Oulu::HexEncode("f", nullptr, ':'), "66")
		TS_ASSERT_EQUALS(Oulu::HexEncode("fo", nullptr, ':'), "66:6f")
		TS_ASSERT_EQUALS(Oulu::HexEncode("foo", nullptr, ':'), "66:6f:6f")
	}

	void TestPercentDecode()
	{
		// Test that we can decode using the default table.
		TS_ASSERT_EQUALS(Oulu::PercentDecode(""), "")
		TS_ASSERT_EQUALS(Oulu::PercentDecode("foo"), "foo")
		TS_ASSERT_EQUALS(Oulu::PercentDecode("foo%20bar"), "foo bar")
		TS_ASSERT_EQUALS(Oulu::PercentDecode("foo%20bar%3F"), "foo bar?")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::PercentDecode("foo%20bar", 6), "foo ")
	}

	void TestPercentEncode()
	{
		// Test that we can decode using the default table.
		TS_ASSERT_EQUALS(Oulu::PercentEncode(""), "")
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo"), "foo")
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo bar"), "foo%20bar")
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo bar?"), "foo%20bar%3F")

		// Test that we can handle truncation.
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo bar", 4), "foo%20")

		// Test that we can encode using an alternate table.
		static constexpr const auto* table = "bar";
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo", table), "%66%6F%6F")
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo bar", table), "%66%6F%6F%20bar")

		// Test that we can encode using the lower-case table.
		TS_ASSERT_EQUALS(Oulu::PercentEncode("foo?", nullptr, false), "foo%3f")
	}
};
