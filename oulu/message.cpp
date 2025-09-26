// Oulu <https://github.com/inspircd/liboulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <oulu/message.hpp>

std::string Oulu::EscapeTag(const std::string_view& str)
{
	std::string ret;
	ret.reserve(str.size());

	for (const auto chr : str)
	{
		switch (chr)
		{
			case ' ':
				ret.append("\\s");
				break;
			case ';':
				ret.append("\\:");
				break;
			case '\\':
				ret.append("\\\\");
				break;
			case '\n':
				ret.append("\\n");
				break;
			case '\r':
				ret.append("\\r");
				break;
			default:
				ret.push_back(chr);
				break;
		}
	}
	return ret;
}

bool Oulu::IsCTCP(const std::string_view& str)
{
	// According to draft-oakley-irc-ctcp-02 a valid CTCP must begin with SOH and
	// contain at least one octet which is not NUL, SOH, CR, LF, or SPACE. As most
	// of these are restricted at the protocol level we only need to check for SOH
	// and SPACE.
	return (str.length() >= 2) && (str[0] == '\x1') && (str[1] != '\x1') && (str[1] != ' ');
}

bool Oulu::ParseCTCP(const std::string_view& str, std::string_view& name)
{
	if (!IsCTCP(str))
	{
		name = {};
		return false;
	}

	auto end_of_name = str.find(' ', 2);
	if (end_of_name == std::string_view::npos)
	{
		// The CTCP only contains a name.
		auto end_of_ctcp = str.back() == '\x1' ? 1 : 0;
		name = str.substr(1, str.length() - end_of_ctcp - 1);
		return true;
	}

	// The CTCP contains a name and a body.
	name = str.substr(1, end_of_name - 1);
	return true;
}

bool Oulu::ParseCTCP(const std::string_view& str, std::string_view& name, std::string_view& body)
{
	if (!IsCTCP(str))
	{
		name = body = {};
		return false;
	}

	auto end_of_name = str.find(' ', 2);
	auto end_of_ctcp = str.back() == '\x1' ? 1 : 0;
	if (end_of_name == std::string_view::npos)
	{
		// The CTCP only contains a name.
		name = str.substr(1, str.length() - end_of_ctcp - 1);
		body = {};
		return true;
	}

	// The CTCP contains a name and a body.
	name = str.substr(1, end_of_name - 1);

	auto start_of_body = str.find_first_not_of(' ', end_of_name + 1);
	if (start_of_body == std::string_view::npos)
	{
		// The CTCP body is provided but empty.
		body = {};
		return true;
	}

	// The CTCP body provided was non-empty.
	body = str.substr(start_of_body, str.length() - start_of_body - end_of_ctcp);
	return true;
}

std::string Oulu::UnescapeTag(const std::string_view& str)
{
	std::string ret;
	ret.reserve(str.size());

	for (auto it = str.cbegin(); it != str.cend(); ++it)
	{
		auto chr = *it;
		if (chr != '\\')
		{
			ret.push_back(chr);
			continue;
		}

		it++;
		if (it == str.cend())
			break;

		chr = *it;
		switch (chr)
		{
			case 's':
				ret.push_back(' ');
				break;
			case ':':
				ret.push_back(';');
				break;
			case '\\':
				ret.push_back('\\');
				break;
			case 'n':
				ret.push_back('\n');
				break;
			case 'r':
				ret.push_back('\r');
				break;
			default:
				ret.push_back(chr);
				break;
		}
	}
	return ret;
}

Oulu::MessageTokenizer::MessageTokenizer(const std::string_view& m)
	: message(m)
{
}

bool Oulu::MessageTokenizer::GetMiddle(std::string_view& token)
{
	// If we are past the end of the string we can't do anything.
	if (this->message.empty())
	{
		token = {};
		return false;
	}

	// If we can't find another separator this is the last token in the message.
	auto separator = this->message.find(' ');
	if (separator == std::string_view::npos)
	{
		token = this->message;
		this->message = {};
		return true;
	}

	token = this->message.substr(0, separator);

	separator = this->message.find_first_not_of(' ', separator);
	if (separator != std::string_view::npos)
		this->message.remove_prefix(separator);

	return true;
}

bool Oulu::MessageTokenizer::GetTrailing(std::string_view& token)
{
	// If we are past the end of the string we can't do anything.
	if (this->message.empty())
	{
		token = {};
		return false;
	}

	// If this is true then we have a <trailing> token!
	if (message.front() == ':')
	{
		token = message.substr(1);
		this->message = {};
		return true;
	}

	// There is no <trailing> token so it must be a <middle> token.
	return this->GetMiddle(token);
}
