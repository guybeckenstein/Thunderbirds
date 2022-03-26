#include<iomanip>
#include "Board.h"
#include "Menu.h"

// Setters

// Creating a board from the screen which has been read
void Board::initializeBoard(const vector<string>& table, const vector<Point>& exitCoord, const Point& legendCoord)
{
	setTable(table);
	setHeight(table.size());
	setWidth(table[0].size());
	setLegend(legendCoord);
	for (size_t y = 0; y < getHeight(); y++)
	{
		if (_table[y] == "")
			for (size_t x = 0; x < getWidth(); x++)
				_table[y].push_back(' ');
		else if (y == legendCoord.getY() || y == legendCoord.getY() + 1 || y == legendCoord.getY() + 2)
			for (int x = legendCoord.getX(); x < legendCoord.getX() + static_cast<int>(boardConstants::LEGEND_MAX_WIDTH); x++)
				if (isWall(x, y) == true)
					_table[y][x] = static_cast<char>(boardConstants::BLANK);
	}
	for (size_t i = 0; i < exitCoord.size(); i++)
		_exit.push_back(exitCoord[i]);
}
// Methods
// Printing current board table
void Board::Print(bool colorAllowed, const string& screenName) const
{
	Menu::gotoxy(0, 0);
	for (size_t y = 0; y < getHeight(); y++)
	{
		for (size_t x = 0; x < getWidth(); x++)
			if (isEmptySpace(x, y) == true || _table[y][x] == static_cast<char>(boardConstants::LEGEND))
				std::cout << static_cast<char>(boardConstants::BLANK);
			else
			{
				if (colorAllowed == true)
					if (_table[y][x] == static_cast<char>(boardConstants::EXIT))
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(boardConstants::EXIT_COLOR));
					else
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::NON_COLOR));
				std::cout << _table[y][x];
			}
		std::cout << std::endl;
	}
	if (colorAllowed == true) // Bonus
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
	std::cout << std::endl << std::setw(getWidth()) << screenName;
}