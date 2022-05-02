#include "Game.h"
#include <cstdlib>

// Constructor and Game Construction
Game::Game(const string& screenName, bool colorAllowed, bool& isValid) : _board()
{
	vector<string> board(static_cast<int>(boardConstants::TABLE_MAX_HEIGHT));
	Point legendCoord;
	vector<Point> exit;
	if (isValidScreen(screenName, board, exit, legendCoord) == true)
	{
		_board.initializeBoard(board, exit, legendCoord);
		// Adding all blocks coordinators is performed in isValidScreen
		_blocksCoord.resize(_blocksCoord.size());
		isValid = true;
	}
	else
	{
		_blocksCoord.clear();
		isValid = false;
	}
}
bool Game::isValidScreen(const string& screenName, vector<string>& currBoard, vector<Point>& exitCoord, Point& legendCoord)
{
	Menu::clearScreen();
	bool res = true, illegalCharacter = false;
	std::fstream currFile(screenName, std::ios::in);
	if (currFile.is_open() == false)
	{
		std::cout << "Invalid screen file '" << screenName << "': The file could not be opened." << std::endl;
		res = false;
	}
	else
	{
		int height = 0, screenWidth = 0, legendHeight = 0, countBigShipCharacters = 0, countSmallShipCharacters = 0, countLegendCharacters = 0, countExitCharacters = 0;
		vector<Point> allBlockCoord;
		string currLine;
		// Header
		std::getline(currFile, currLine, '\n');
		setTime(std::stoi(currLine));
		std::getline(currFile, currLine, '\n');
		// Table itself
		int currLineWidth;
		for (auto y = 0; std::getline(currFile, currLine, '\n') && res == true && illegalCharacter == false; y++)
		{
			if (y == 0)
				screenWidth = setScreenWidth(currLine, screenWidth, res);
			// Reading each line and its elements (walls, blocks and entities)
			currLineWidth = readTextFileLine(allBlockCoord, exitCoord, currBoard, y, currLine, screenWidth, countBigShipCharacters, countSmallShipCharacters, countLegendCharacters, countExitCharacters, legendHeight, legendCoord, illegalCharacter);
			// Adding blank characters for lines which are shorter than board's screenWidth
			if (currLine[currLineWidth] == '\0')
				while (currLineWidth++ < screenWidth)
					currBoard[y].push_back(' ');
			height = y;
			res = isValidAmountOfCharacters(countBigShipCharacters, countSmallShipCharacters, countLegendCharacters);
		}
		// Adding whole blank lines, for different cases
		if (height == legendHeight)
		{
			int tmpHeight = height + 1;
			for (int k = 0; k < screenWidth; k++)
			{
				currBoard[height].push_back(' ');
				currBoard[tmpHeight].push_back(' ');
			}
			height += 3;
		}
		else if (height == legendHeight + 1)
		{
			for (int k = static_cast<int>(currBoard[height].size()); k < screenWidth; k++)
				currBoard[height].push_back(' ');
			height += 2;
		}
		else
			height += 1;
		currBoard.resize(height);
		res = createVectorsFromPoints(allBlockCoord);
		isInvalidScreen(screenName, res, illegalCharacter, height, screenWidth, countBigShipCharacters, countSmallShipCharacters, countLegendCharacters, countExitCharacters);
		if (res == true && illegalCharacter == false)
			seperateEntitiesFromBoard(allBlockCoord, currBoard);
	}
	currFile.close();
	Menu::clearScreen();
	return res;
}
// Checking only the first line, for setting the width of the board. If the width is 0, the board is invalid
int Game::setScreenWidth(const string& firstLine, int screenWidth, bool& res)
{
	if (firstLine == "")
		res = false;
	else if (isLegendOnFirstLine(firstLine, screenWidth) == true)
	{
		screenWidth += static_cast<int>(boardConstants::LEGEND_MAX_WIDTH);
		if (screenWidth < static_cast<int>(firstLine.size()))
			screenWidth = static_cast<int>(firstLine.size());
	}
	else
		screenWidth = static_cast<int>(firstLine.size());
	return screenWidth;
}
// Function indicates through a boolean value if the first line of the input file includes a legend '&' 
bool Game::isLegendOnFirstLine(const string& string, int& width) const
{
	for (size_t y = 0; y < string.size(); y++)
		if (string[y] == static_cast<char>(boardConstants::LEGEND))
		{
			width = static_cast<int>(y);
			return true;
		}
	return false;
}
// Reads the current line of the text file and its elements (walls, blocks and entities)
int Game::readTextFileLine(vector<Point>& allBlockCoord, vector<Point>& allExitCoord, vector<string>& currBoard, int y, const string& currLine, int screenWidth, int& countBigShip, int& countSmallShip, int& countLegend, int& countExitCharacters, int& legendHeight, Point& legendCoord, bool& illegalCharacter)
{
	int x = 0;
	for (; illegalCharacter == false && currLine[x] != '\0' && x < screenWidth; x++)
	{
		currBoard[y].push_back(currLine[x]);
		if (currLine[x] == static_cast<char>(bigShipConstants::FIGURE))
		{
			_bigShipCoord.push_back({ x, y });
			countBigShip++;
		}
		else if (currLine[x] == static_cast<char>(smallShipConstants::FIGURE))
		{
			_smallShipCoord.push_back({ x, y });
			countSmallShip++;
		}
		else if (currLine[x] == static_cast<char>(boardConstants::BLOCK))
			allBlockCoord.push_back({ x, y });
		else if (currLine[x] == static_cast<char>(horizontalGhostConstants::FIGURE))
			_horizontalGhostsCoord.push_back({ x, y });
		else if (currLine[x] == static_cast<char>(verticalGhostConstants::FIGURE))
			_verticalGhostsCoord.push_back({ x, y });
		else if (currLine[x] == static_cast<char>(wanderingGhostConstants::FIGURE))
			_wanderingGhostsCoord.push_back({ x, y });
		else if (currLine[x] == static_cast<char>(boardConstants::EXIT))
		{
			allExitCoord.push_back({ x,y });
			countExitCharacters++;
		}
		else if (currLine[x] == static_cast<char>(boardConstants::LEGEND))
		{
			legendHeight = y;
			legendCoord.setX(x);
			legendCoord.setY(y);
			countLegend++; // Only 1 character, exactly
		}
		// In this game implementation, only used characters by ASCII table values are: 32, 33, 35, 36, 37, 38, 42, 64, 69, 87. All other characters on screen are illegal
		if ((0 < currLine[x] && currLine[x] < 32) || currLine[x] == 34 || 
			(38 < currLine[x] && currLine[x] < 42) || (42 < currLine[x] && currLine[x] < 64) || 
			(64 < currLine[x] && currLine[x] < 69) || (69 < currLine[x] && currLine[x] < 87) || 87 < currLine[x])
			illegalCharacter = true;
	}
	return x;
}
// Seperating all coords to their appropriate vector
bool Game::createVectorsFromPoints(const vector<Point>& allBlockCoord)
{
	bool res = createBigShipVector() && createSmallShipVector();
	if (res == true)
		createBlocksVector(allBlockCoord);
	return res;
}
// Checks if the coordinators of the big ship are spread within four adjacent characters, two characters in a row of two columns
bool Game::createBigShipVector()
{
	if (_bigShipCoord.size() == static_cast<int>(bigShipConstants::HEIGHT) * static_cast<int>(bigShipConstants::WIDTH) && 
		(_bigShipCoord[0].getX() + 1 == _bigShipCoord[1].getX() && _bigShipCoord[0].getY() == _bigShipCoord[1].getY() &&
		_bigShipCoord[0].getX() == _bigShipCoord[2].getX() && _bigShipCoord[0].getY() + 1 == _bigShipCoord[2].getY() &&
		_bigShipCoord[0].getX() + 1 == _bigShipCoord[3].getX() && _bigShipCoord[0].getY() + 1 == _bigShipCoord[3].getY()))
	{
		return true;
	}
	_bigShipCoord.clear();
	return false;
}
// Checks if the coordinators of the small ship are spread within two adjacent characters, one character in a row of two columns
bool Game::createSmallShipVector()
{
	if (_smallShipCoord.size() == static_cast<int>(smallShipConstants::HEIGHT) * static_cast<int>(smallShipConstants::WIDTH) && 
		(_smallShipCoord[0].getX() + 1 == _smallShipCoord[1].getX() && _smallShipCoord[0].getY() == _smallShipCoord[1].getY()))
	{
		return true;
	}
	_smallShipCoord.clear();
	return false;
}
// Creates all the blocks - which means that each block would be defined by a vector of points
void Game::createBlocksVector(const vector<Point>& allBlocksCoord)
{
	bool isAdjacent = true;
	vector<bool> isMarkedCoord(allBlocksCoord.size());
	vector<Point> currVector = { allBlocksCoord[0] };
	isMarkedCoord[0] = true;
	for (size_t i = 1; i < allBlocksCoord.size(); i++)
	{
		if (isMarkedCoord[i] == false)
		{
			while (isAdjacent == true)
			{
				isAdjacent = false;
				for (size_t j = i; j < allBlocksCoord.size() && isAdjacent == false; j++)
				{
					if (isMarkedCoord[j] == true)
						continue;
					for (size_t k = 0; k < currVector.size() && isAdjacent == false; k++)
					{
						if (abs(currVector[k].getX() - allBlocksCoord[j].getX()) + abs(currVector[k].getY() - allBlocksCoord[j].getY()) == 1)
						{
							currVector.push_back(allBlocksCoord[j]);
							isMarkedCoord[j] = true;
							isAdjacent = true;
						}
					}
				}
			}
			// Adding previous vector of points
			if (currVector.size() > 0)
			{
				_blocksCoord.push_back(currVector);
				currVector.clear();
			}
			// Creating a new vector of points
			if (isMarkedCoord[i] == false)
			{
				currVector.push_back(allBlocksCoord[i]);
				isMarkedCoord[i] = true;
			}
		}
		isAdjacent = true;
	}
	if (allBlocksCoord.size() == 1)
		_blocksCoord.push_back(currVector);
}
// Erasing ships and blocks from the board itself, so there will not be static characters on the board
void Game::seperateEntitiesFromBoard(const vector<Point>& allBlockCoord, vector<string>& currBoard) const
{
	for (const auto& currCoord : getBigShipCoord())
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
	for (const auto& currCoord : getSmallShipCoord())
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
	for (const auto& currCoord : allBlockCoord)
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
	for (const auto& currCoord : _horizontalGhostsCoord)
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
	for (const auto& currCoord : _verticalGhostsCoord)
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
	for (const auto& currCoord : _wanderingGhostsCoord)
		currBoard[currCoord.getY()][currCoord.getX()] = static_cast<char>(boardConstants::BLANK);
}
// Updates 'res' (output parameter of function) if one of the if clauses is true, and prints an appropriate message for any case
void Game::isInvalidScreen(const string& screenName, bool& res, bool illegalCharacter, int height, int width, int countBigShip, int countSmallShip, int countLegend, int countExitCharacters) const
{
	if (illegalCharacter == true)
	{
		std::cout << "Invalid screen file '" << screenName << "': contains an illegal character." << std::endl;
		res = false;
	}
	else if (width == 0)
	{
		std::cout << "Invalid screen file '" << screenName << "': empty screen." << std::endl;
		res = false;
	}
	else if (height > static_cast<int>(boardConstants::TABLE_MAX_HEIGHT))
	{
		std::cout << "Invalid screen file '" << screenName << "': too large horizontally." << std::endl;
		res = false;
	}
	else if (width > static_cast<int>(boardConstants::TABLE_MAX_WIDTH))
	{
		std::cout << "Invalid screen file '" << screenName << "': too large vertically." << std::endl;
		res = false;
	}
	else if (countBigShip != static_cast<int>(bigShipConstants::HEIGHT) * static_cast<int>(bigShipConstants::WIDTH))
	{
		std::cout << "Invalid screen file '" << screenName << "': does not contain just one valid big ship." << std::endl;
		res = false;
	}
	else if (countSmallShip != static_cast<int>(smallShipConstants::HEIGHT) * static_cast<int>(smallShipConstants::WIDTH))
	{
		std::cout << "Invalid screen file '" << screenName << "': does not contain just one valid small ship." << std::endl;
		res = false;
	}
	else if (countLegend != 1)
	{
		std::cout << "Invalid screen file '" << screenName << "': does not contain just one legend area." << std::endl;
		res = false;
	}
	else if (countExitCharacters == 0)
	{
		std::cout << "Invalid screen file '" << screenName << "': does not contain any exit characters." << std::endl;
		res = false;
	}
	if (res == false)
		Menu::pauseScreen();
}
// Operators
std::ostream& operator<<(std::ostream& out, Direction direction)
{
	if (direction == Direction::UP)
		return out << 0;
	else if (direction == Direction::LEFT)
		return out << 1;
	else if (direction == Direction::DOWN)
		return out << 2;
	else if (direction == Direction::RIGHT)
		return out << 3;
	return out << -1;
}
// Game Methods
// Loop continue running until player wins, or loses
bool Game::Play(const string& screenName, BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, int& lives, int& time, bool colorAllowed)
{
	bool shipsEscaped = false;
	bool exit = false; // If player pauses game with ESC, then decides to quit by pressing 9, he would get back to menu
	getBoard().Print(colorAllowed, screenName);
	bigShip.Print(colorAllowed, static_cast<int>(bigShipConstants::COLOR));
	smallShip.Print(colorAllowed, static_cast<int>(smallShipConstants::COLOR));
	for (size_t i = 0; i < blocks.size(); i++)
		blocks[i].Print(colorAllowed, static_cast<int>(blockConstants::COLOR));
	for (size_t i = 0; i < horizontalGhosts.size(); i++)
		horizontalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	for (size_t i = 0; i < verticalGhosts.size(); i++)
		verticalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	for (size_t i = 0; i < wanderingGhosts.size(); i++)
		wanderingGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	while (lives > 0 && shipsEscaped == false && exit == false) // Unless the game resets 3 times (either a heavy block touches a ship or no more ticks) or both ships has reached exit points
	{
		// Printing player's lives and score status every loop
		Print(lives, time, bigShip.getActive(), colorAllowed);
		bool isShipDie = moveShipsAndBlocks(bigShip, smallShip, blocks, horizontalGhosts, verticalGhosts, wanderingGhosts, createGhostsCoordVector(horizontalGhosts, verticalGhosts, wanderingGhosts), exit, colorAllowed) || moveGhosts(bigShip.getCoord(), smallShip.getCoord(), blocks, horizontalGhosts, verticalGhosts, wanderingGhosts, colorAllowed);
		if (isShipDie == false)
		{
			shipsEscaped = bigShip.getHasReachedExit() && smallShip.getHasReachedExit();
			Sleep(static_cast<int>(gameConstants::GAME_SPEED));
			time--;
		}
		if (shipsEscaped == false && (isShipDie == true || time == 0))
		{
			getBoard().Print(colorAllowed, screenName);
			Reset(bigShip, smallShip, blocks, horizontalGhosts, verticalGhosts, wanderingGhosts, time, lives);
			bigShip.Print(colorAllowed, static_cast<int>(bigShipConstants::COLOR));
			smallShip.Print(colorAllowed, static_cast<int>(smallShipConstants::COLOR));
			for (size_t i = 0; i < blocks.size(); i++)
				blocks[i].Print(colorAllowed, static_cast<int>(blockConstants::COLOR));
			for (size_t i = 0; i < horizontalGhosts.size(); i++)
				horizontalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
			for (size_t i = 0; i < verticalGhosts.size(); i++)
				verticalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
			for (size_t i = 0; i < wanderingGhosts.size(); i++)
				wanderingGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
		}
	}
	// End of loop -> means that the player has won or lost.
	Outcome(lives, exit, getBoard().getLegend().getX(), getBoard().getLegend().getY(), colorAllowed);
	return exit;
}
// Create a vector of points (or vector of 1x1 vector of points) from the coordinators of all ghost types - horizontal, vertical and wandering
vector<Point> Game::createGhostsCoordVector(const vector<HorizontalGhost>& horizontalGhosts, const vector<VerticalGhost>& verticalGhosts, const vector<WanderingGhost>& wanderingGhosts) const
{
	vector<Point> allGhostsCoord;
	for (size_t i = 0; i < horizontalGhosts.size(); i++)
		allGhostsCoord.push_back(horizontalGhosts[i].getCoord()[0]);
	for (size_t i = 0; i < verticalGhosts.size(); i++)
		allGhostsCoord.push_back(verticalGhosts[i].getCoord()[0]);
	for (size_t i = 0; i < wanderingGhosts.size(); i++)
		allGhostsCoord.push_back(wanderingGhosts[i].getCoord()[0]);
	return allGhostsCoord;
}
// Updates the coordinator of the ships and blocks, returns true if a heavy block fells on a ship
bool Game::moveShipsAndBlocks(BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, const vector<Point>& allGhosts, bool& exit, bool colorAllowed)
{
	bool validMove = true, isBlockHeavierThanShip = false;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i].eraseRender(blocks[i].getCoord());
		if (blocks[i].updateBlockOnAir(getBoard(), bigShip.getCoord(), smallShip.getCoord(), blocks, allGhosts, 0) == true) // If a block is heavier than either one of ships
			return true;
	}
	for (size_t i = 0; i < blocks.size(); i++)
	{
		for (size_t ghostIdx = 0; i < horizontalGhosts.size(); i++)
			if (horizontalGhosts[ghostIdx].isBlockClash(blocks[i], Direction::UNDEFINED) == true)
				horizontalGhosts[ghostIdx].setCoord({ getHorizontalGhostsCoord()[ghostIdx] }); // A horizontal ghost is dead
		for (size_t ghostIdx = 0; i < verticalGhosts.size(); i++)
			if (verticalGhosts[ghostIdx].isBlockClash(blocks[i], Direction::UNDEFINED) == true)
				verticalGhosts[ghostIdx].setCoord({ getVerticalGhostsCoord()[ghostIdx] }); // A vertical ghost is dead
		for (size_t ghostIdx = 0; i < wanderingGhosts.size(); i++)
			if (wanderingGhosts[ghostIdx].isBlockClash(blocks[i], Direction::UNDEFINED) == true)
				wanderingGhosts[ghostIdx].setCoord({ getWanderingGhostsCoord()[ghostIdx] }); // A wandering ghost is dead
	}
	if (bigShip.getActive() == true)
	{
		vector<Point> tmpBigShipCoord = bigShip.getCoord();
		exit = bigShip.Move(getBoard(), smallShip.getCoord(), blocks, allGhosts, validMove, smallShip.getHasReachedExit(), colorAllowed);
		if (bigShip.getActive() == false)
			smallShip.setActive(true);
		if (validMove == true)
		{
			bigShip.eraseRender(tmpBigShipCoord);
			for (size_t i = 0; i < bigShip.getCoord().size() && bigShip.getActive() == true && bigShip.getHasReachedExit() == false; i++)
				bigShip.setHasReachedExit(hasReachedExit(bigShip.getCoord()[i], getBoard().getExit()));
			if (bigShip.getHasReachedExit() == false)
				bigShip.Print(colorAllowed, static_cast<int>(bigShipConstants::COLOR));
			else
			{
				bigShip.removeEntityExistance();
				smallShip.setActive(true);
				if (smallShip.getHasReachedExit() == true)
					return false;
			}
		}
		else
			bigShip.setCoord(tmpBigShipCoord);
	}
	else if(smallShip.getActive() == true)
	{
		vector<Point> tmpSmallShipCoord = smallShip.getCoord();
		exit = smallShip.Move(getBoard(), bigShip.getCoord(), blocks, allGhosts, validMove, bigShip.getHasReachedExit(), colorAllowed);
		if (smallShip.getActive() == false)
			bigShip.setActive(true);
		if (validMove == true)
		{
			smallShip.eraseRender(tmpSmallShipCoord);
			for (size_t i = 0; i < smallShip.getCoord().size() && smallShip.getActive() == true && smallShip.getHasReachedExit() == false; i++)
				smallShip.setHasReachedExit(hasReachedExit(smallShip.getCoord()[i], getBoard().getExit()));
			if (smallShip.getHasReachedExit() == false)
				smallShip.Print(colorAllowed, static_cast<int>(smallShipConstants::COLOR));
			else
			{
				smallShip.removeEntityExistance();
				bigShip.setActive(true);
				if (bigShip.getHasReachedExit() == true)
					return false;
			}
		}
		else
			smallShip.setCoord(tmpSmallShipCoord);
	}
	for (size_t i = 0; i < blocks.size(); i++)
		blocks[i].Print(colorAllowed, static_cast<int>(blockConstants::COLOR));
	return false;
}
// If a ship has got to the exit spot
bool Game::hasReachedExit(const Point& coord, const vector<Point> exit) const
{
	for (const auto& itr : exit)
		if (coord == itr)
			return true;
	return false;
}
// Update the coordinator of the ghosts, returns true if a ghost touches one of the ships
bool Game::moveGhosts(const vector<Point>& bigShipCoord, const vector<Point>& smallShipCoord, const vector<Block>& allBlocksCoord, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, bool colorAllowed)
{
	// Horizontal ghost
	for (size_t i = 0; i < horizontalGhosts.size(); i++)
	{
		// Before ghost-number-i's move
		if (horizontalGhosts[i].isShipClash(bigShipCoord) == true || horizontalGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		horizontalGhosts[i].eraseRender(horizontalGhosts[i].getCoord());
		horizontalGhosts[i].Move(getBoard(), allBlocksCoord);
		// After ghost-number-i's move
		if (horizontalGhosts[i].isShipClash(bigShipCoord) == true || horizontalGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		horizontalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	}
	// Vertical ghost
	for (size_t i = 0; i < verticalGhosts.size(); i++)
	{
		// Before ghost-number-i's move
		if (verticalGhosts[i].isShipClash(bigShipCoord) == true || verticalGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		verticalGhosts[i].eraseRender(verticalGhosts[i].getCoord());
		verticalGhosts[i].Move(getBoard(), allBlocksCoord);
		// After ghost-number-i's move
		if (verticalGhosts[i].isShipClash(bigShipCoord) == true || verticalGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		verticalGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	}
	// Wandering ghost
	for (size_t i = 0; i < wanderingGhosts.size(); i++)
	{
		// Before ghost-number-i's move
		if (wanderingGhosts[i].isShipClash(bigShipCoord) == true || wanderingGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		wanderingGhosts[i].eraseRender(wanderingGhosts[i].getCoord());
		wanderingGhosts[i].Move(getBoard(), allBlocksCoord);
		// After ghost-number-i's move
		if (wanderingGhosts[i].isShipClash(bigShipCoord) == true || wanderingGhosts[i].isShipClash(smallShipCoord) == true)
			return true;
		wanderingGhosts[i].Print(colorAllowed, static_cast<int>(ghostConstants::COLOR));
	}
	return false;
}
// Function's output is the final screen message that is printed, after each game screen
void Game::Outcome(int lives, bool isQuit, int legendX, int legendY, bool colorAllowed) const
{
	string str1, str2, str3;
	if (lives == 0)
	{
		str1 = "YOU LOST!";
		str2 = "Better luck next time.";
	}
	else if (isQuit == true)
	{
		str1 = "YOU QUIT!";
		str2 = "Never give up.";
	}
	else
	{
		str1 = "YOU WON!";
		str2 = "Congratulations.";
	}
	// Output padding
	while (str1.size() < static_cast<int>(boardConstants::LEGEND_MAX_WIDTH))
		str1 += ' ';
	while (str2.size() < static_cast<int>(boardConstants::LEGEND_MAX_WIDTH))
		str2 += ' ';
	while (str3.size() < static_cast<int>(boardConstants::LEGEND_MAX_WIDTH))
		str3 += ' ';
	// Output
	if (colorAllowed == true)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
	Menu::gotoxy(legendX, legendY);
	std::cout << str1 << std::endl;
	Menu::gotoxy(legendX, legendY + 1);
	std::cout << str2 << std::endl;
	Menu::gotoxy(legendX, legendY + 2);
	std::cout << str3 << std::endl;
	Sleep(5 * static_cast<int>(gameConstants::GAME_SPEED));
	Menu::clearScreen();
	std::cout << str1 << std::endl << str2 << std::endl;
	Menu::pauseScreen();
}
// General Game Methods
// Function for displaying legend message
void Game::Print(int lives, int time, bool isBigShipActive, bool colorAllowed) const
{
	Menu::gotoxy(getBoard().getLegend().getX(), getBoard().getLegend().getY());
	if (colorAllowed == true)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(menuConstants::COLOR));
	std::cout << "REMAINING LIVES: " << lives << std::endl;
	Menu::gotoxy(getBoard().getLegend().getX(), getBoard().getLegend().getY() + 1);
	std::cout << "TIME LEFT FOR THIS SCREEN: " << time << "  "; // Whitespace padding
	Menu::gotoxy(getBoard().getLegend().getX(), getBoard().getLegend().getY() + 2);
	if (isBigShipActive == true)
		std::cout << "ACTIVE SHIP: BIG SHIP  "; // Whitespace padding
	else
		std::cout << "ACTIVE SHIP: SMALL SHIP";
}
// Used when initializing or resetting the game (when time ends or a heavy block lands on a ship)
void Game::Reset(BigShip& bigShip, SmallShip& smallShip, vector<Block>& blocks, vector<HorizontalGhost>& horizontalGhosts, vector<VerticalGhost>& verticalGhosts, vector<WanderingGhost>& wanderingGhosts, int& time, int& lives) 
{
	// Big ship resetting
	bigShip.Reset(getBigShipCoord());
	bigShip.setHasReachedExit(false);
	// Small ship resetting
	smallShip.Reset(getSmallShipCoord());
	smallShip.setHasReachedExit(false);
	// Blocks resetting
	for (size_t i = 0; i < blocks.size(); i++)
		blocks[i].Reset(getBlocksCoord()[i]);
	// Ghosts resetting
	for (size_t i = 0; i < horizontalGhosts.size(); i++)
		horizontalGhosts[i].Reset({ getHorizontalGhostsCoord()[i] });
	for (size_t i = 0; i < verticalGhosts.size(); i++)
		verticalGhosts[i].Reset({ getVerticalGhostsCoord()[i] });
	for (size_t i = 0; i < wanderingGhosts.size(); i++)
		wanderingGhosts[i].Reset({ getWanderingGhostsCoord()[i] });
	// Print resetting
	Menu::clearLegendArea(getBoard().getLegend());
	// Time resetting and amount of lives update
	time = getTime();
	lives--;
}