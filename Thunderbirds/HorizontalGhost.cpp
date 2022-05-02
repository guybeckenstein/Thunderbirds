#include "HorizontalGhost.h"

// Moves horizontally, left to right and back, on the row where it was born initially (as set by the screen designer)
bool HorizontalGhost::getPossibleDirections(const Board& board, const vector<Block>& allBlocks, vector<bool>& newPossibleDirections) const
{
	for(int i = 0; i < static_cast<int>(horizontalGhostConstants::NUM_OF_DIRECTIONS); i++)
		newPossibleDirections.push_back(true);

	if (getDirection() == Direction::RIGHT || getCoord()[0].getX() == 0)
		newPossibleDirections[0] = false; // Can not go LEFT
	if (getDirection() == Direction::LEFT || getCoord()[0].getX() == board.getWidth() - 1)
		newPossibleDirections[1] = false; // Can not go RIGHT
	checkForWalls(board, newPossibleDirections);
	for (size_t i = 0; i < allBlocks.size(); i++)
	{
		if (isBlockClash(allBlocks[i], Direction::LEFT) == true)
			newPossibleDirections[0] = false;
		if (isBlockClash(allBlocks[i], Direction::RIGHT) == true)
			newPossibleDirections[1] = false;
	}
	return newPossibleDirections != Ghost::getPossibleDirections();
}
// Checks for walls around the ghost
void HorizontalGhost::checkForWalls(const Board& board, vector<bool>& newPossibleDirections) const
{
	int x = getCoord()[0].getX(), y = getCoord()[0].getY();
	if (newPossibleDirections[0] == true && (board.isWall(x - 1, y) == true || board.isExit(x - 1, y) == true))
		newPossibleDirections[0] = false; // Can not go LEFT
	if (newPossibleDirections[1] == true && (board.isWall(x + 1, y) == true || board.isExit(x + 1, y) == true))
		newPossibleDirections[1] = false; // Can not go RIGHT
}
// Generates a random direction for ghost
void HorizontalGhost::getNextDirection(Direction& newDirection) const
{
	if (Ghost::getPossibleDirections()[0] != false || Ghost::getPossibleDirections()[1] != false)
	{
		Direction oldDirection = newDirection;
		int randomValue = rand() % static_cast<int>(horizontalGhostConstants::NUM_OF_DIRECTIONS);
		if (randomValue == 0)
			newDirection = Direction::LEFT;
		else
			newDirection = Direction::RIGHT;
		while (Ghost::getPossibleDirections()[randomValue] == false)
		{
			newDirection = oldDirection;
			randomValue = rand() % static_cast<int>(horizontalGhostConstants::NUM_OF_DIRECTIONS);
			if (randomValue == 0)
				newDirection = Direction::LEFT;
			else
				newDirection = Direction::RIGHT;
		}
	}
	else
		newDirection = Direction::UNDEFINED;
}