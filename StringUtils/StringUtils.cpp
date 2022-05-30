#include "StringUtils.h"

namespace StringUtils
{

	void removeComments(std::string& str)
	{
		const std::string COMMENTS_START = "//";
		std::size_t pos = str.find(COMMENTS_START);
		// Check if index position is valid 
		if (pos != std::string::npos)
		{
			// Remove all characters after the given position 
			str = str.substr(0, pos);
		}
	}
	void trim(std::string& str)
	{
		const char* typeOfWhitespaces = " \t\n\r\f\v";
		str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
		str.erase(0, str.find_first_not_of(typeOfWhitespaces));
	}
	std::vector<std::string> splitByDelimiter(std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> args;

		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos)
		{
			token = str.substr(0, pos);
			args.push_back(token);
			str.erase(0, pos + delimiter.length());
		}
		args.push_back(str);

		return args;
	}

	std::vector<std::string> getArgs(std::string& line)
	{
		removeComments(line);
		trim(line);
		return splitByDelimiter(line, " ");
	}
}