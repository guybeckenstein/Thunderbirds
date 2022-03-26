#include <filesystem>
#include <fstream>
#include "InputParser.h"
#include "Menu.h"

// Compares the current file name to the format we are needed to work with.
// The game would look for files in the working directory, with the names 'tb*.screen'.

int main(int argc, char* argv[])
{
    Menu::clearScreen();
    int numOfScreens = 0;
    vector<string> screenNames;

    for (auto const& cwd_entry : std::filesystem::directory_iterator{ "textFiles" }) // The game would look for files in working directory->textFiles
    {
        if (InputParser::validName(cwd_entry.path().string(), "screen") == true)
        {
            screenNames.push_back(cwd_entry.path().string());
            numOfScreens++;
        }
    }
    std::sort(screenNames.begin(), screenNames.end()); // Sorting files in lexicographical order
    if (numOfScreens > 0)
    {
        Menu initialize(screenNames);
        initialize.Run();
    }
    else
        std::cout << "Could not find any appropriate files with the names tb*.screen, hence the game could not be played." << std::endl;
    return 0;
}