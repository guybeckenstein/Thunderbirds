// This class is used for reading Thunderbirds game files, that are later formatting the game screens.
#pragma once

#include <string>
#include <vector>

using std::vector, std::string;

class InputParser
{
private:
    vector <string> _tokens;
public:
    // Constructor
    InputParser(int& argc, char** argv); // Ex3
    // Other Methods
    const string& getCmdOption(const string& option) const; // Ex3
    static bool validName(const string& name, const string& type); // Ex1 + Ex2
};