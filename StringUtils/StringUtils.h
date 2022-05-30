#include <iostream>
#include <string>
#include <vector>

namespace StringUtils
{
	void removeComments(std::string& str);
	void trim(std::string& str);
	std::vector<std::string> splitByDelimiter(std::string& str, const std::string& delimiter);
	std::vector<std::string> getArgs(std::string& line);
}
