#include "VerticalGhost.h"

// Moves vertically, up and down, on the column where it was born initially (as set by the screen designer)
bool VerticalGhost::getPossibleDirections(const Board& board, const vector<Block>& allBlocks, vector<bool>& newPossibleDirections) const
{
	for (int i = 0; i < static_cast<int>(verticalGhostConstants::NUM_OF_DIRECTIONS); i++)
		newPossibleDirections.push_back(true);

	if (getDirection() == Direction::DOWN || getCoord()[0].getY() == 0)
		newPossibleDirections[0] = false; // Can not go UP
	if (getDirection() == Direction::UP || getCoord()[0].getY() == board.getHeight() - 1)
		newPossibleDirections[1] = false; // Can not go DOWN
	checkForWalls(board, newPossibleDirections);
	for (size_t i = 0; i < allBlocks.size(); i++)
	{
		if (isBlockClash(allBlocks[i], Direction::UP) == true)
			newPossibleDirections[0] = false;
		if (isBlockClash(allBlocks[i], Direction::DOWN) == true)
			newPossibleDirections[1] = false;
	}
	return newPossibleDirections != Ghost::getPossibleDirections();
}
// Checks for walls around the ghost
void VerticalGhost::checkForWalls(const Board& board, vector<bool>& newPossibleDirections) const
{
	int x = getCoord()[0].getX(), y = getCoord()[0].getY();
	if (newPossibleDirections[0] == true && (board.isWall(x, y - 1) == true || board.isExit(x, y - 1) == true))
		newPossibleDirections[0] = false; // Can not go UP
	if (newPossibleDirections[1] == true && (board.isWall(x, y + 1) == true || board.isExit(x, y + 1) == true))
		newPossibleDirections[1] = false; // Can not go DOWN
}
// Generates a random direction for ghost
void VerticalGhost::getNextDirection(Direction& newDirection) const
{
	if (Ghost::getPossibleDirections()[0] != false || Ghost::getPossibleDirections()[1] != false)
	{
		Direction oldDirection = newDirection;
		int randomValue = rand() % static_cast<int>(verticalGhostConstants::NUM_OF_DIRECTIONS);
		if (randomValue == 0)
			newDirection = Direction::UP;
		else
			newDirection = Direction::DOWN;
		while (Ghost::getPossibleDirections()[randomValue] == false)
		{
			newDirection = oldDirection;
			randomValue = rand() % static_cast<int>(verticalGhostConstants::NUM_OF_DIRECTIONS);
			if (randomValue == 0)
				newDirection = Direction::UP;
			else
				newDirection = Direction::DOWN;
		}
	}
	else
		newDirection = Direction::UNDEFINED;
}