#include <filesystem>
#include <fstream>
#include "Menu.h"

// Compares the current file name to the format we are needed to work with.
// The game would look for files in the working directory, with the names 'tb*.screen'.

int main(int argc, char* argv[])
{
    Menu::clearScreen();
    vector<string> screenNames;

    // The game would look for files in our working directory (.\\textFiles\\)
    for (auto const& cwd_entry : std::filesystem::directory_iterator{ "textFiles" })
        if (Menu::validScreenName(cwd_entry.path().string()) == true) // Looking for screen files
            screenNames.push_back(cwd_entry.path().string());
    std::sort(screenNames.begin(), screenNames.end()); // Sorting screen files in lexicographical order
    if (screenNames.size() > 0)
    {
        Menu initialize(screenNames);
        initialize.Run();
    }
    else
        std::cout << "Could not find any appropriate files with the names tb*.screen, hence the game could not be played." << std::endl;
    return 0;
}