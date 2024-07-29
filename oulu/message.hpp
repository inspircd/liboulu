// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>

namespace Oulu
{
	class MessageTokenizer;

	/** Escapes a string to the IRCv3 tag format.
	 * \param str The string to escape.
	 */
	std::string EscapeTag(const std::string_view& str);

	/** Unescapes a string from the IRCv3 tag format.
	 * \param str The string to unescape.
	 */
	std::string UnescapeTag(const std::string_view& str);
}

/** MessageTokenizer allows tokens in the IRC wire format to be read from a message. */
class Oulu::MessageTokenizer final
{
private:
	/** The message we are parsing tokens from. */
	std::string_view message;

public:
	/** Creates a MessageTokenizer for the specified message. */
	MessageTokenizer(const std::string_view& m);

	/** Retrieve the next \<GetMiddle> token in the message.
	 * \param token The next token available, or an empty string view if none remain.
	 * \return True if a token was retrieved; otherwise, false.
	 */
	bool GetMiddle(std::string_view& token);

	/** Retrieve the next \<GetTrailing> token in the message.
	 * \param token The next token available, or an empty string view if none remain.
	 * \return True if a token was retrieved; otherwise, false.
	 */
	bool GetTrailing(std::string_view& token);
};
