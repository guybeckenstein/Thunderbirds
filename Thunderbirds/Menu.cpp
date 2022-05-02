#include <filesystem>
#include "Game.h"
#include "Menu.h"

// Constants
constexpr char ONE = '1', TWO = '2', EIGHT = '8', NINE = '9'; // Numbers constants for all menus

// Compares the current file name to the format we are needed to work with
// The game would look for files in the working directory, with the names tb*.screen
bool Menu::validScreenName(const string& name)
{
	if (name.compare(strlen("textFiles\\"), strlen("tb"), "tb") == 0)
	{
		string part;
		for (size_t i = strlen("textFiles\\") + strlen("tb"); name[i] != '.'; i++)
			part.push_back(name[i]);
		if (name.compare(strlen("textFiles\\") + strlen("tb"), part.size() + 1 + 6, part + "." + "screen") == 0)
			return true;
	}
	return false;
}
// Initializes and runs the game completely
void Menu::Run()
{
	char choice = mainMenu(); // Prints menu
	bool exitGame = false;
	while (exitGame == false)
	{
		if (choice == ONE)
			newGameMenu();
			//loadScreens();
		else if (choice == TWO)
			setColorSettings(colorScreen(getColorSettings())); // Sets the new color settings of Game class by calling Menu class' colorSettings function. This function gets a boolean value from the current Game class colorSettings getter
		else if (choice == EIGHT)
			Instructions();
		else if (choice == NINE)
			exitGame = true;
		if (exitGame == false)
		{
			clearScreen();
			choice = mainMenu();
		}
	}
}
// Prints the first main menu of the game. This menu appears after first initialization, after exiting Thunderbirds's game and after picking the color settings or the instructions
char Menu::mainMenu()
{
	std::cout << "(1) Start a new game" << std::endl
		<< "(2) Add or remove color" << std::endl
		<< "(8) Present instructions and keys" << std::endl 
		<< "(9) EXIT" << std::endl;
	char choice = _getch();
	while (choice != ONE && choice != TWO && choice != EIGHT && choice != NINE)
	{
		gotoxy(0, 3);
		std::cout << std::endl << std::endl << "Wrong number, choose one of the numbers above.";
		gotoxy(0, 2);
		choice = _getch();
	}
	return choice;
}
// Prints the second menu of the game. This menu appears after picking 'start a new game' option on main menu
void Menu::newGameMenu() const
{
	clearScreen();
	std::cout << "(1) Start a full game from beginning" << std::endl
		<< "(2) Start a single game by screen name" << std::endl
		<< "(9) EXIT to main menu" << std::endl;
	char choice = _getch();
	while (choice != ONE && choice != TWO && choice != NINE)
	{
		gotoxy(0, 3);
		std::cout << std::endl << std::endl << "Wrong number, choose one of the numbers above.";
		gotoxy(0, 2);
		choice = _getch();
	}
	if (choice == ONE)
		loadScreens(0);
	else if (choice == TWO)
		loadScreens(getScreenIndexByName());
	else // if (choice == NINE)
		pauseScreen();
}
int Menu::getScreenIndexByName() const
{
	clearScreen();
	std::cout << "All game screens found at working directory of the game are below:" << std::endl;
	for (size_t i = 0; i < getScreenNames().size(); i++)
	{
		std::cout << "(" << i + 1 << ") " << getScreenNames()[i].substr(10) << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Write down the index of the single screen you would like to play." << std::endl 
		<< "Index '0' is relevant if you regret and want to start a full game." << std::endl;
	char index = _getch() - '0';
	// If an invalid index is given
	while (static_cast<size_t>(index) > getScreenNames().size())
	{
		gotoxy(0, 1 + static_cast<int>(getScreenNames().size()) + 1);
		std::cout << std::endl << std::endl << "Wrong index, choose one of the indexes above.";
		gotoxy(0, 1 + static_cast<int>(getScreenNames().size()));
		index = _getch() - '0';
	}
	gotoxy(0, 1 + static_cast<int>(getScreenNames().size()) + 1 + 2);
	std::cout << "                                                     ";
	gotoxy(0, 1 + static_cast<int>(getScreenNames().size()) + 1 + 2);
	pauseScreen();
	return index;
}
void Menu::loadScreens(int singleScreenIndex) const
{
	bool isWin = true, isValidScreen = false, exit = false, fullGame = true;
	int remainingLives = static_cast<int>(gameConstants::INIT_LIVES), remainingTime = 0;
	size_t numOfScreenNames = getScreenNames().size();
	for (size_t i = 0; fullGame == true && exit == false && isWin == true && i < numOfScreenNames; i++)
	{
		int gameIndex = (singleScreenIndex == 0) ? gameIndex = static_cast<int>(i) : gameIndex = singleScreenIndex - 1;
		Game screen(getScreenNames()[gameIndex], getColorSettings(), isValidScreen);
		remainingTime = screen.getTime();
		if (isValidScreen == true)
		{
			// Initializing ships
			BigShip bigShip = BigShip(screen.getBigShipCoord());
			SmallShip smallShip = SmallShip(screen.getSmallShipCoord());
			// Initializing blocks
			vector<Block> blocks;
			for (size_t blockIdx = 0; blockIdx < screen.getBlocksCoord().size(); blockIdx++)
				blocks.push_back(Block(screen.getBlocksCoord()[static_cast<int>(blockIdx)], static_cast<int>(blockIdx)));
			// Initializing ghosts
			vector<HorizontalGhost> horizontalGhosts;
			for (size_t ghostIdx = 0; ghostIdx < screen.getHorizontalGhostsCoord().size(); ghostIdx++)
				horizontalGhosts.push_back(HorizontalGhost({ screen.getHorizontalGhostsCoord()[static_cast<int>(ghostIdx)] }, { false , false }));
			vector<VerticalGhost> verticalGhosts;
			for (size_t ghostIdx = 0; ghostIdx < screen.getVerticalGhostsCoord().size(); ghostIdx++)
				verticalGhosts.push_back(VerticalGhost({ screen.getVerticalGhostsCoord()[static_cast<int>(ghostIdx)] }, { false , false }));
			vector<WanderingGhost> wanderingGhosts;
			for (size_t ghostIdx = 0; ghostIdx < screen.getWanderingGhostsCoord().size(); ghostIdx++)
				wanderingGhosts.push_back(WanderingGhost({ screen.getWanderingGhostsCoord()[static_cast<int>(ghostIdx)] }, { false , false , false , false }));
			// Initializing game
			exit = screen.Play(getScreenNames()[gameIndex], bigShip, smallShip, blocks, horizontalGhosts, verticalGhosts, wanderingGhosts, remainingLives, remainingTime, getColorSettings());
			if (remainingLives == 0)
				isWin = false;
			if (singleScreenIndex != 0)
				fullGame = false;
		}
		else
		{
			std::cout << "Invalid screen: '" << getScreenNames()[gameIndex] << "'. Game over" << std::endl;
			pauseScreen();
			return;
		}
	}
	if (fullGame == true)
		Results(isWin, exit);
}
void Menu::Results(bool isWin, bool isQuit) const
{
	clearScreen();
	// Player QUIT
	if (isQuit == true)
		std::cout << "An excuse is the most expensive brand of self defeat you will ever purchase." << std::endl
		<< "If you quit on the process, you are quitting on the result." << std::endl
		<< "And remember for life - quitting is never an option." << std::endl;
	// Player WON
	else if (isWin == true)
		std::cout << "Congratulations for winning the challenging Thunderbirds game!" << std::endl
		<< "Your triumph will ring through the history of MTA College." << std::endl;
	// Player LOST
	else
		std::cout << "You lost... But remember:" << std::endl
		<< "Sometimes you must lose everything to gain it again," << std::endl
		<< "and the regaining is the sweeter for the pain of loss." << std::endl;
	pauseScreen();
}
void Menu::Instructions() // Displays the player all game controls and rules
{
	clearScreen();
	std::cout << "The player controls one ship of two - a big one (represented with '#' characters) and a small one (represented with '@' characters)." << std::endl 
		<< "The ships are stuck inside an Egyptian tomb and their goal is to get out of it as soon as possible." << std::endl
		<< "Finally, the player must avoid all blocks, whether they are small or big (represented with '*' characters)." << std::endl << std::endl
		<< "Game controls in the game are below:" << std::endl
		<< "W or w - go UP." << std::endl << "A or a - go LEFT." << std::endl << "D or d - go RIGHT." << std::endl << "X or x - go DOWN." << std::endl
		<< "B - Switched to the Big Ship (if we were with this ship already, just STOP the movement of this ship)."<< std::endl
		<< "S - Switched to the Small Ship (if we were with this ship already, just STOP the movement of this ship)" << std::endl
		<< "ESC - PAUSE game. ESC again - CONTINUE game. If you want to exit game, you can press 9 after initially pressing ESC." << std::endl
		<< "If you have changed your desired color, you can pick a new color again, through pressing (2) in menu." << std::endl << std::endl;
	pauseScreen();
}
void Menu::pauseGame(const Point& legend, bool colorAllowed, bool& exit) // If player will press "ESC" button -> game pauses. If player will press "ESC" again -> game continues
{
	gotoxy(legend.getX(), legend.getY());
	if (colorAllowed == true) // Bonus
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
	std::cout << "Game paused. Press ESC   " << std::endl; // Legend max width is 25
	gotoxy(legend.getX(), legend.getY() + 1);
	std::cout << "again to continue, or    " << std::endl; // Legend max width is 25
	gotoxy(legend.getX(), legend.getY() + 2);
	std::cout << "press 9 to exit.         " << std::endl; // Legend max width is 25
	char key = 0;
	while (key != static_cast<int>(gameConstants::ESC) && key != static_cast<int>(gameConstants::EXIT))
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == static_cast<int>(gameConstants::ESC) || key == static_cast<int>(gameConstants::EXIT))
			{
				if (key == static_cast<int>(gameConstants::EXIT))
					exit = true;
				clearLegendArea(legend);
				continue;
			}
		}
	}
}
void Menu::clearLegendArea(const Point& legend)
{
	gotoxy(legend.getX(), legend.getY());
	for (auto i = 0; i < static_cast<int>(boardConstants::LEGEND_MAX_HEIGHT); i++)
	{
		for (auto j = 0; j < static_cast<int>(boardConstants::LEGEND_MAX_WIDTH); j++)
			std::cout << " "; // Whitespace padding
		std::cout << std::endl;
		gotoxy(legend.getX(), legend.getY() + i + 1);
	}
}
// Operators
const Menu& Menu::operator=(const Menu& menu)
{
	if (this != &menu)
	{
		setColorSettings(menu._colorAllowed);
		_screenNames.clear();
		_screenNames = menu._screenNames;
	}
	return *this;
}
// We are expected to use gotoxy
void Menu::gotoxy(int x, int y)
{
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	// Hides cursor
	CONSOLE_CURSOR_INFO info = { 100, false };
	SetConsoleCursorInfo(hConsoleOutput, &info);
	COORD dwCursorPosition = { static_cast<short>(x), static_cast<short>(y) };
	// Sets cursor position
	std::cout << std::flush;
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}
// Bonus Methods
// This function changes color settings of the game, if player decides to change it.
bool Menu::colorScreen(bool isColorAllowed)
{
	clearScreen();
	bool newColorSettings;
	if (isColorAllowed == true)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
	std::cout << "You can choose whether you will play in a colorful mode or not" << std::endl
		<< "(0) Non-color mode" << std::endl
		<< "(1) Color mode" << std::endl;

	char choice = _getch();
	while (choice != '0' && choice != '1')
	{
		gotoxy(0, 4);
		std::cout << "Wrong number, choose the numbers above.";
		gotoxy(0, 3);
		choice = _getch();
	}
	gotoxy(0, 6);
	if (choice == '0')
	{
		newColorSettings = false;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::NON_COLOR));
		std::cout << "Initializing non-color mode..." << std::endl;
	}
	else
	{
		newColorSettings = true;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
		std::cout << "Initializing color mode..." << std::endl;
	}
	pauseScreen();
	return newColorSettings;
}