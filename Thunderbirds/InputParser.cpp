#include "InputParser.h"

// Constructor
InputParser::InputParser(int& argc, char** argv)
{
	for (int i = 1; i < argc; i++) // The constructor puts all string-argument values in the vector of the class
		_tokens.push_back(string(argv[i]));
}
// Methods checks if the string input appears in the vector of the class. If it does - it returns the string input, else - it returns a blank string
const string& InputParser::getCmdOption(const string& option) const
{
	static const string empty_string("");
	if (std::find(_tokens.begin(), _tokens.end(), option) == _tokens.end())
		return empty_string;
	return option;
}
// Compares the current file name to the format we are needed to work with
// The game would look for files in the working directory, with the names thunderbirds_*.screen
bool InputParser::validName(const string& name, const string& type)
{
	if (name.compare(strlen("textFiles\\"), strlen("tb"), "tb") == 0)
	{
		string part;
		for (size_t i = strlen("Screens\\") + strlen("tb"); name[i] != '.'; i++)
			part.push_back(name[i]);
		if (name.compare(strlen("Screens\\") + strlen("tb"), part.size() + 1 + type.size(), part + "." + type) == 0)
			return true;
	}
	return false;
}