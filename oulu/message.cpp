// Oulu <https://github.com/SadieCat/oulu/>
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
