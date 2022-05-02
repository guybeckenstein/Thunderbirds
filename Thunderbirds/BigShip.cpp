#include "BigShip.h"
#include "Game.h"

// Returns true only if player quits game
bool BigShip::Move(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, const vector<Point>& allGhosts, bool& validMove, bool hasReachedExit, bool colorAllowed)
{
	static Direction nextDirection = Direction::UNDEFINED; // nextDirection is used to "remember" ship's direction if its active
	static bool cantProgress;
	bool exit = false;
	char key = getArrowKeys(board, allBlocks, colorAllowed, exit);
	if (getFirstMove() == true)
	{
		nextDirection = Direction::UNDEFINED;
		setFirstMove(false);
	}
	vector<Point> pointsToUpdate = getCoord();
	cantProgress = getNextDirection(board, smallShip, allBlocks, allGhosts, key, nextDirection, hasReachedExit);
	if (cantProgress == false)
	{
		for (size_t i = 0; i < pointsToUpdate.size(); i++)
			pointsToUpdate[i].updateCoord(nextDirection);
		setCoord(pointsToUpdate);
	}
	else
	{
		validMove = false;
		setDirection(Direction::BIG_SHIP_OR_STAY);
	}
	return exit;
}
// Returns the key value of the button that the player pressed on keyboard
char BigShip::getArrowKeys(const Board& board, const vector<Block>& allBlocks, bool colorAllowed, bool& exit) const
{
	static char key;
	if (getFirstMove() == true)
		key = 0;
	if (_kbhit())
	{
		key = _getch();
		if (key == static_cast<char>(gameConstants::ESC))
		{
			for (size_t i = 0; i < allBlocks.size(); i++)
				allBlocks[i].Print(colorAllowed, static_cast<int>(blockConstants::COLOR));
			Menu::pauseGame(board.getLegend(), colorAllowed, exit);
			for (size_t i = 0; i < allBlocks.size(); i++)
				allBlocks[i].eraseRender(allBlocks[i].getCoord());
		}
	}
	if (key == 's' || key == 'S')
	{
		key = 0;
		return 's';
	}
	return key;
}
// Returns the direction of the ship
bool BigShip::getNextDirection(const Board& board, const vector<Point>& smallShip, vector<Block>& allBlocks, const vector<Point>& allGhosts, char key, Direction& nextDirection, bool hasReachedExit)
{
	bool cantProgress = false;
	vector<Block> allBlocksTmp = allBlocks; // Used if a blocked-by-wall ship tries to push a block, but it actually can't
	for (size_t i = 0; i < getCoord().size() && getActive() == true && cantProgress == false; i++)
	{
		int x = getCoord()[i].getX(), y = getCoord()[i].getY();
		if (key == static_cast<char>(gameConstants::ESC))
			cantProgress = true;
		else if (key == 'w' || key == 'W')
		{
			if (i > 1)
				continue;
			if (y == 0 || board.isWall(x, y - 1) == true || areEntitiesClashing({ x, y - 1 }, smallShip) == true)
				cantProgress = true;
			else
			{
				for (size_t j = 0; j < allBlocks.size() && cantProgress == false; j++)
				{
					if (isBlockClash(allBlocks[j], Direction::UP) == true)
					{
						if (static_cast<int>(bigShipConstants::MAX_WEIGHT) >= allBlocks[j].getWeight())
							cantProgress = !allBlocks[j].updatePushedBlock(board, smallShip, Direction::UP, allBlocks, allGhosts, static_cast<int>(bigShipConstants::MAX_WEIGHT), allBlocks[j].getWeight());
						else
							cantProgress = true;
					}
				}
				if (cantProgress == true)
					break;
				setDirection(Direction::UP); // Can go UP
				nextDirection = Direction::UP;
			}
		}
		else if (key == 'x' || key == 'X')
		{
			if (i < 2)
				continue;
			if (y == board.getHeight() - 1 || board.isWall(x, y + 1) == true || areEntitiesClashing({ x, y + 1 }, smallShip) == true)
				cantProgress = true;
			else
			{
				for (size_t j = 0; j < allBlocks.size() && cantProgress == false; j++)
				{
					if (isBlockClash(allBlocks[j], Direction::DOWN) == true)
					{
						cantProgress = true;
						setDirection(Direction::BIG_SHIP_OR_STAY); // Can't go DOWN
						nextDirection = Direction::BIG_SHIP_OR_STAY;
					}
				}
				if (cantProgress == false)
				{
					setDirection(Direction::DOWN); // Can go DOWN
					nextDirection = Direction::DOWN;
				}
			}
		}
		else if (key == 'a' || key == 'A')
		{
			if (i == 1 || i == 3)
				continue;
			if (x == 0 || board.isWall(x - 1, y) == true || areEntitiesClashing({ x - 1, y }, smallShip) == true)
				cantProgress = true;
			else
			{
				for (size_t j = 0; j < allBlocks.size() && cantProgress == false; j++)
				{
					if (isBlockClash(allBlocks[j], Direction::LEFT) == true)
					{
						if (static_cast<int>(bigShipConstants::MAX_WEIGHT) >= allBlocks[j].getWeight())
							cantProgress = !allBlocks[j].updatePushedBlock(board, smallShip, Direction::LEFT, allBlocks, allGhosts, static_cast<int>(bigShipConstants::MAX_WEIGHT), allBlocks[j].getWeight());
						else
							cantProgress = true;
					}
				}
				if (cantProgress == true)
					break;
				setDirection(Direction::LEFT); // Can go LEFT
				nextDirection = Direction::LEFT;
			}
		}
		else if (key == 'd' || key == 'D')
		{
			if (i == 0 || i == 2)
				continue;
			if (x == board.getWidth() - 1 || board.isWall(x + 1, y) == true || areEntitiesClashing({ x + 1, y }, smallShip) == true)
				cantProgress = true;
			else
			{
				for (size_t j = 0; j < allBlocks.size() && cantProgress == false; j++)
				{
					if (isBlockClash(allBlocks[j], Direction::RIGHT) == true)
					{
						if (static_cast<int>(bigShipConstants::MAX_WEIGHT) >= allBlocks[j].getWeight())
							cantProgress = !allBlocks[j].updatePushedBlock(board, smallShip, Direction::RIGHT, allBlocks, allGhosts, static_cast<int>(bigShipConstants::MAX_WEIGHT), allBlocks[j].getWeight());
						else
							cantProgress = true;
					}
				}
				if (cantProgress == true)
					break;
				setDirection(Direction::RIGHT); // Can go RIGHT
				nextDirection = Direction::RIGHT;
			}
		}
		else if (key == 'b' || key == 'B' || y == 0 || y == board.getHeight() - 1 || x == 0 || x == board.getWidth() - 1)
		{
			cantProgress = true;
			setDirection(Direction::BIG_SHIP_OR_STAY); // Can STAY
			nextDirection = Direction::BIG_SHIP_OR_STAY;
		}
		else if (hasReachedExit == false && (key == 's' || key == 'S'))
		{
			cantProgress = false;
			setDirection(Direction::SMALL_SHIP_OR_STAY); // Can switch to small ship
			nextDirection = Direction::SMALL_SHIP_OR_STAY;
			setActive(false);
		}
		else
		{
			setDirection(Direction::BIG_SHIP_OR_STAY); // Can STAY
			nextDirection = Direction::BIG_SHIP_OR_STAY;
		}
	}
	if (cantProgress == true)
		allBlocks = allBlocksTmp;
	return cantProgress;
}