#include "WanderingGhost.h"

// Wanders on screen, according to the algorithm that you would implement, it may have some randomness, but should be totally random.
// Algorithm: wanders 100% randomly, without any counter. Each step is random
bool WanderingGhost::getPossibleDirections(const Board& board, const vector<Block>& allBlocks, vector<bool>& newPossibleDirections) const
{
	for (int i = 0; i < static_cast<int>(wanderingGhostConstants::NUM_OF_DIRECTIONS); i++)
		newPossibleDirections.push_back(true);

	if (getDirection() == Direction::DOWN || getCoord()[0].getY() == 0)
		newPossibleDirections[0] = false; // Can not go UP
	if (getDirection() == Direction::RIGHT || getCoord()[0].getX() == 0)
		newPossibleDirections[1] = false; // Can not go LEFT
	if (getDirection() == Direction::UP || getCoord()[0].getY() == board.getHeight() - 1)
		newPossibleDirections[2] = false; // Can not go DOWN
	if (getDirection() == Direction::LEFT || getCoord()[0].getX() == board.getWidth() - 1)
		newPossibleDirections[3] = false; // Can not go RIGHT
	checkForWalls(board, newPossibleDirections);
	for (size_t i = 0; i < allBlocks.size(); i++)
	{
		if (isBlockClash(allBlocks[i], Direction::UP) == true)
			newPossibleDirections[0] = false;
		if (isBlockClash(allBlocks[i], Direction::LEFT) == true)
			newPossibleDirections[1] = false;
		if (isBlockClash(allBlocks[i], Direction::DOWN) == true)
			newPossibleDirections[2] = false;
		if (isBlockClash(allBlocks[i], Direction::RIGHT) == true)
			newPossibleDirections[3] = false;
	}
	return newPossibleDirections != Ghost::getPossibleDirections();
}
// Checks for walls around the ghost
void WanderingGhost::checkForWalls(const Board& board, vector<bool>& newPossibleDirections) const
{
	int x = getCoord()[0].getX(), y = getCoord()[0].getY();
	if (newPossibleDirections[0] == true && (board.isWall(x, y - 1) == true || board.isExit(x, y - 1) == true))
		newPossibleDirections[0] = false; // Can not go UP
	if (newPossibleDirections[1] == true && (board.isWall(x - 1, y) == true || board.isExit(x - 1, y) == true))
		newPossibleDirections[1] = false; // Can not go LEFT
	if (newPossibleDirections[2] == true && (board.isWall(x, y + 1) == true || board.isExit(x, y + 1) == true))
		newPossibleDirections[2] = false; // Can not go DOWN
	if (newPossibleDirections[3] == true && (board.isWall(x + 1, y) == true || board.isExit(x + 1, y) == true))
		newPossibleDirections[3] = false; // Can not go RIGHT
}
// Generates a random direction for ghost
void WanderingGhost::getNextDirection(Direction& newDirection) const
{
	if (Ghost::getPossibleDirections()[0] != false || Ghost::getPossibleDirections()[1] != false || Ghost::getPossibleDirections()[2] != false || Ghost::getPossibleDirections()[3] != false)
	{
		Direction oldDirection = newDirection;
		int randomValue = rand() % static_cast<int>(wanderingGhostConstants::NUM_OF_DIRECTIONS);
		if (randomValue == 0)
			newDirection = Direction::UP;
		else if (randomValue == 1)
			newDirection = Direction::LEFT;
		else if (randomValue == 2)
			newDirection = Direction::DOWN;
		else
			newDirection = Direction::RIGHT;
		while (Ghost::getPossibleDirections()[randomValue] == false)
		{
			newDirection = oldDirection;
			randomValue = rand() % static_cast<int>(wanderingGhostConstants::NUM_OF_DIRECTIONS);
			if (randomValue == 0)
				newDirection = Direction::UP;
			else if (randomValue == 1)
				newDirection = Direction::LEFT;
			else if (randomValue == 2)
				newDirection = Direction::DOWN;
			else
				newDirection = Direction::RIGHT;
		}
	}
	else
		newDirection = Direction::UNDEFINED;
}