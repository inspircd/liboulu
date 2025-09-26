// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>

namespace Oulu
{
	/** The table used when handling regular Base64-encoded strings. */
	inline constexpr const char* BASE64_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	/** The table used when handling Base64URL-encoded strings. */
	inline constexpr const char* BASE64_URL_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

	/** The table used for encoding as a lower-case hexadecimal string. */
	inline constexpr const char* HEX_TABLE_LOWER = "0123456789abcdef";

	/** The table used for encoding as an upper-case hexadecimal string. */
	inline constexpr const char* HEX_TABLE_UPPER = "0123456789ABCDEF";

	/** The table used to determine what characters are safe within a percent-encoded string. */
	inline constexpr const char* PERCENT_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~";

	/** Decodes a Base64-encoded byte array.
	 * \param data The byte array to decode from.
	 * \param length The length of the byte array.
	 * \param table The index table to use for decoding.
	 * \return The decoded form of the specified data.
	 */
	std::string Base64Decode(const void* data, size_t length, const char* table = nullptr);

	/** Decodes a Base64-encoded string.
	 * \param data The string view decode from.
	 * \param table The index table to use for decoding.
	 * \return The decoded form of the specified data.
	 */
	inline std::string Base64Decode(const std::string_view& data, const char* table = nullptr)
	{
		return Base64Decode(data.data(), data.length(), table);
	}

	/** Encodes a byte array using Base64.
	 * \param data The byte array to encode from.
	 * \param length The length of the byte array.
	 * \param table The index table to use for encoding.
	 * \param padding If non-zero then the character to pad encoded strings with.
	 * \return The encoded form of the specified data.
	 */
	std::string Base64Encode(const void* data, size_t length, const char* table = nullptr, char padding = '=');

	/** Encodes a string using Base64.
	 * \param data The string view encode from.
	 * \param table The index table to use for encoding.
	 * \param padding If non-zero then the character to pad encoded strings with.
	 * \return The encoded form of the specified data.
	 */
	inline std::string Base64Encode(const std::string_view& data, const char* table = nullptr, char padding = '=')
	{
		return Base64Encode(data.data(), data.length(), table, padding);
	}

	/** Decodes a hexadecimal-encoded byte array.
	 * \param data The byte array to decode from.
	 * \param length The length of the byte array.
	 * \param separator If non-zero then the character hexadecimal digits are separated with.
	 * \param table The index table to use for decoding.
	 * \return The decoded form of the specified data.
	 */
	std::string HexDecode(const void* data, size_t length, const char* table = nullptr, char separator = 0);

	/** Decodes a hexadecimal-encoded string.
	 * \param data The string view decode from.
	 * \param table The index table to use for decoding.
	 * \param separator If non-zero then the character hexadecimal digits are separated with.
	 * \return The decoded form of the specified data.
	 */
	inline std::string HexDecode(const std::string_view& data, const char* table = nullptr, char separator = 0)
	{
		return HexDecode(data.data(), data.length(), table, separator);
	}

	/** Encodes a byte array using hexadecimal encoding.
	 * \param data The byte array to encode from.
	 * \param length The length of the byte array.
	 * \param table The index table to use for encoding.
	 * \param separator If non-zero then the character to separate hexadecimal digits with.
	 * \return The encoded form of the specified data.
	 */
	std::string HexEncode(const void* data, size_t length, const char* table = nullptr, char separator = 0);

	/** Encodes a string using Base64.
	 * \param data The string view encode from.
	 * \param table The index table to use for encoding.
	 * \param separator If non-zero then the character to separate hexadecimal digits with.
	 * \return The encoded form of the specified data.
	 */
	inline std::string HexEncode(const std::string_view& data, const char* table = nullptr, char separator = 0)
	{
		return HexEncode(data.data(), data.length(), table, separator);
	}

	/** Decodes a percent-encoded byte array.
	 * \param data The byte array to decode from.
	 * \param length The length of the byte array.
	 * \return The decoded form of the specified data.
	 */
	std::string PercentDecode(const void* data, size_t length);

	/** Decodes a percent-encoded string.
	 * \param data The string view decode from.
	 * \return The decoded form of the specified data.
	 */
	inline std::string PercentDecode(const std::string_view& data)
	{
		return PercentDecode(data.data(), data.length());
	}

	/** Encodes a byte array using percent encoding.
	 * \param data The byte array to encode from.
	 * \param length The length of the byte array.
	 * \param table The table of characters that do not require escaping.
	 * \param upper Whether to use upper or lower case.
	 * \return The encoded form of the specified data.
	 */
	std::string PercentEncode(const void* data, size_t length, const char* table = nullptr, bool upper = true);

	/** Encodes a string using percent encoding.
	 * \param data The string view encode from.
	 * \param table The table of characters that do not require escaping.
	 * \param upper Whether to use upper or lower case.
	 * \return The encoded form of the specified data.
	 */
	inline std::string PercentEncode(const std::string_view& data, const char* table = nullptr, bool upper = true)
	{
		return PercentEncode(data.data(), data.length(), table, upper);
	}
}
