// Oulu <https://github.com/SadieCat/oulu/>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <oulu/message.hpp>

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
