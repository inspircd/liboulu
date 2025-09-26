// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <cstring>
#include <cstdint>

#include <oulu/encoding.hpp>

std::string Oulu::Base64Decode(const void* data, size_t length, const char* table)
{
	if (!table)
		table = Oulu::BASE64_TABLE;

	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve((length / 4) * 3);

	uint32_t current_bits = 0;
	size_t seen_bits = 0;

	const auto* cdata = static_cast<const char*>(data);
	for (size_t idx = 0; idx < length; ++idx)
	{
		// Attempt to find the octet in the table.
		const auto* chr = strchr(table, cdata[idx]);
		if (!chr)
			continue; // Skip invalid octets.

		// Add the bits for this octet to the active buffer.
		current_bits = (current_bits << 6) | uint32_t(chr - table);
		seen_bits += 6;

		if (seen_bits >= 8)
		{
			// We have seen an entire octet; add it to the buffer.
			seen_bits -= 8;
			buffer.push_back((current_bits >> seen_bits) & 0xFF);
		}
	}

	return buffer;
}

std::string Oulu::Base64Encode(const void* data, size_t length, const char* table, char padding)
{
	// Use the default table if one is not specified.
	if (!table)
		table = Oulu::BASE64_TABLE;

	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve(4 * ((length + 2) / 3));

	const auto* udata = static_cast<const uint8_t*>(data);
	for (size_t idx = 0; idx < length; )
	{
		// Base64 encodes three octets into four characters.
		uint32_t octet1 = idx < length ? udata[idx++] : 0;
		uint32_t octet2 = idx < length ? udata[idx++] : 0;
		uint32_t octet3 = idx < length ? udata[idx++] : 0;
		uint32_t triple = (octet1 << 16) + (octet2 << 8) + octet3;

		buffer.push_back(table[(triple >> 3 * 6) & 63]);
		buffer.push_back(table[(triple >> 2 * 6) & 63]);
		buffer.push_back(table[(triple >> 1 * 6) & 63]);
		buffer.push_back(table[(triple >> 0 * 6) & 63]);
	}

	static constexpr size_t padding_count[] = { 0, 2, 1 };
	if (padding)
	{
		// Replace any trailing characters with padding.
		for (size_t idx = 0; idx < padding_count[length % 3]; ++idx)
			buffer[buffer.length() - 1 - idx] = '=';
	}
	else
	{
		// Remove any trailing characters.
		buffer.erase(buffer.length() - padding_count[length % 3]);
	}

	return buffer;
}

std::string Oulu::HexDecode(const void* data, size_t length, const char* table, char separator)
{
	if (!table)
		table = Oulu::HEX_TABLE_LOWER;

	// The size of each hex segment.
	size_t segment = (separator ? 3 : 2);

	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve(length / segment);

	const auto* cdata = static_cast<const char*>(data);
	for (size_t idx = 0; idx + 1 < length; idx += segment)
	{
		// Attempt to find the octets in the table.
		const auto* table1 = strchr(table, cdata[idx]);
		const auto* table2 = strchr(table, cdata[idx + 1]);

		const auto pair = ((table1 ? table1 - table : 0) << 4)
			+ (table2 ? table2 - table : 0);
		buffer.push_back(pair);
	}

	return buffer;
}

std::string Oulu::HexEncode(const void* data, size_t length, const char* table, char separator)
{
	if (!table)
		table = Oulu::HEX_TABLE_LOWER;

	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve((length * 2) + (!!separator * length));

	const auto* udata = reinterpret_cast<const unsigned char*>(data);
	for (size_t idx = 0; idx < length; ++idx)
	{
		if (idx && separator)
			buffer.push_back(separator);

		const auto chr = udata[idx];
		buffer.push_back(table[chr >> 4]);
		buffer.push_back(table[chr & 15]);
	}

	return buffer;
}

std::string Oulu::PercentDecode(const void* data, size_t length)
{
	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve(length * 3);

	const auto* cdata = reinterpret_cast<const char*>(data);
	for (size_t idx = 0; idx < length; ++idx)
	{
		if (cdata[idx] == '%')
		{
			// Percent encoding encodes two octets into 1-2 characters.
			const char octet1 = ++idx < length ? toupper(cdata[idx]) : 0;
			const char octet2 = ++idx < length ? toupper(cdata[idx]) : 0;

			const auto* table1 = strchr(Oulu::HEX_TABLE_UPPER, octet1);
			const auto* table2 = strchr(Oulu::HEX_TABLE_UPPER, octet2);

			const auto pair = ((table1 ? table1 - Oulu::HEX_TABLE_UPPER : 0) << 4)
				+ (table2 ? table2 - Oulu::HEX_TABLE_UPPER : 0);
			buffer.push_back(pair);
		}
		else
		{
			buffer.push_back(cdata[idx]);
		}
	}

	return buffer;
}

std::string Oulu::PercentEncode(const void* data, size_t length, const char* table, bool upper)
{
	if (!table)
		table = Oulu::PERCENT_TABLE;

	// Preallocate the output buffer to avoid constant reallocations.
	std::string buffer;
	buffer.reserve(length * 3);

	const auto* hex_table = upper ? Oulu::HEX_TABLE_UPPER : Oulu::HEX_TABLE_LOWER;
	const auto* udata = reinterpret_cast<const unsigned char*>(data);
	for (size_t idx = 0; idx < length; ++idx)
	{
		const auto chr = udata[idx];
		if (strchr(table, chr))
		{
			// The character is on the safe list; push it as is.
			buffer.push_back(chr);
		}
		else
		{
			// The character is not on the safe list; percent encode it.
			buffer.push_back('%');
			buffer.push_back(hex_table[chr >> 4]);
			buffer.push_back(hex_table[chr & 15]);
		}
	}

	return buffer;
}
